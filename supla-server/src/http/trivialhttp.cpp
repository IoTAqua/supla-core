/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#define OUTDATA_MAXSIZE 102400
#define INDATA_MAXSIZE 102400
#define METHOD_MAXSIZE 20
#define IN_BUFFER_SIZE 1024
#define TOKEN_MAXSIZE 2048

#include <arpa/inet.h>
#include <http/trivialhttp.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "log.h"

supla_trivial_http::supla_trivial_http(const char *host, const char *resource) {
  this->sfd = -1;
  this->resultCode = 0;
  this->contentLength = 0;
  this->contentType = NULL;
  this->port = 80;
  this->host = host ? strndup(host, HOST_MAXSIZE) : NULL;
  this->resource = resource ? strndup(resource, RESOURCE_MAXSIZE) : NULL;
  this->body = NULL;
  this->token = NULL;
}

supla_trivial_http::supla_trivial_http(void) {
  this->sfd = -1;
  this->resultCode = 0;
  this->contentLength = 0;
  this->contentType = NULL;
  this->port = 80;
  this->host = NULL;
  this->resource = NULL;
  this->body = NULL;
  this->token = NULL;
}

supla_trivial_http::~supla_trivial_http(void) {
  setHost(NULL);
  setResource(NULL);
  releaseResponse();
  setToken(NULL, false);
}

void supla_trivial_http::set_string_variable(char **var, int max_len, char *src,
                                             bool copy) {
  if (*var) {
    free(*var);
    *var = NULL;
  }

  if (src && strnlen(src, max_len) > 0) {
    *var = copy ? strndup(src, max_len) : src;
  } else if (src && !copy) {
    free(src);
  }
}

void supla_trivial_http::releaseResponse(void) {
  set_string_variable(&body, 0, NULL, false);
  set_string_variable(&contentType, 0, NULL, false);

  this->resultCode = 0;
  this->contentLength = 0;
}

void supla_trivial_http::setHost(char *host, bool copy) {
  set_string_variable(&this->host, 1024, host, copy);
}

void supla_trivial_http::setPort(int port) { this->port = port; }

void supla_trivial_http::setResource(char *resource, bool copy) {
  set_string_variable(&this->resource, 1024, resource, copy);
}

int supla_trivial_http::getResultCode(void) { return resultCode; }

int supla_trivial_http::getContentLength(void) { return contentLength; }

const char *supla_trivial_http::getContentType(void) { return contentType; }

const char *supla_trivial_http::getBody(void) { return body; }

bool supla_trivial_http::get_addrinfo(void **res) {
  if (!res) {
    return false;
  }

  *res = NULL;

  struct in6_addr serveraddr;
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_NUMERICSERV;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  char port[15];
  snprintf(port, sizeof(port), "%i", this->port);

  char empty[] = "";
  char *server = empty;

  if (host != NULL && strnlen(host, HOST_MAXSIZE) > 0) {
    server = host;
  }

  if (inet_pton(AF_INET, server, &serveraddr) == 1) {
    hints.ai_family = AF_INET;
    hints.ai_flags |= AI_NUMERICHOST;
  } else if (inet_pton(AF_INET6, server, &serveraddr) == 1) {
    hints.ai_family = AF_INET6;
    hints.ai_flags |= AI_NUMERICHOST;
  }

  return getaddrinfo(server, port, &hints, (struct addrinfo **)res) == 0;
}

ssize_t supla_trivial_http::_write(void *ptr, const void *__buf, size_t __n) {
  return write(*(int *)ptr, __buf, __n);
}

ssize_t supla_trivial_http::_read(void *ptr, void *__buf, size_t __n) {
  return read(*(int *)ptr, __buf, __n);
}

bool supla_trivial_http::_should_retry(void *ptr) { return false; }

void supla_trivial_http::write_read(void *ptr, const char *out, char **in) {
  if (_write(ptr, out, strnlen(out, OUTDATA_MAXSIZE)) >= 0) {
    char in_buff[IN_BUFFER_SIZE];
    int size = 0;
    int read_len = 0;

    while ((read_len = _read(ptr, in_buff, sizeof(IN_BUFFER_SIZE))) > 0 ||
           _should_retry(ptr)) {
      if (read_len > 0 && size + read_len < INDATA_MAXSIZE) {
        *in = (char *)realloc((void *)*in, size + read_len + 1);
        if (*in == NULL) {
          break;
        }
        memcpy(&((*in)[size]), in_buff, read_len);
        size += read_len;
      }
    }

    if (*in) {
      // size+read_len+1
      (*in)[size] = 0;
    }
  }
}

bool supla_trivial_http::send_recv(const char *out, char **in) {
  bool result = false;

  struct addrinfo *ai = NULL;

  if (!get_addrinfo((void **)&ai)) {
    return false;
  }

  *in = NULL;

  sfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
  if (sfd >= 0 &&
      connect(sfd, ai->ai_addr, (unsigned int)ai->ai_addrlen) == 0) {
    write_read(&sfd, out, in);
  }

  if (sfd != -1) {
    close(sfd);
    sfd = -1;
  }

  if (ai) {
    freeaddrinfo(ai);
    ai = NULL;
  }

  return result;
}

char *supla_trivial_http::header_item_match(const char *item, unsigned int size,
                                            const char *name,
                                            unsigned int name_size) {
  if (size >= name_size && memcmp(item, name, name_size) == 0) {
    char *i = (char *)item;
    return &i[name_size];
  }

  return NULL;
}

void supla_trivial_http::parse_header_item(const char *item, unsigned int size,
                                           bool *chunked) {
  char _http[] = "HTTP/1.1 ";
  char _contentType[] = "Content-Type: ";
  char _contentLength[] = "Content-Length: ";
  char _transferEncoding[] = "Transfer-Encoding: chunked";

  char *match = NULL;
  if (NULL !=
      (match = header_item_match(item, size, _http, sizeof(_http) - 1))) {
    char *space = NULL;
    if ((space = strstr(match, " ")) != NULL) {
      space[0] = 0;
      resultCode = atoi(match);
    }
  } else if (NULL != (match = header_item_match(item, size, _contentType,
                                                sizeof(_contentType) - 1))) {
    if (contentType) {
      free(contentType);
    }
    contentType = strdup(match);

  } else if (chunked && NULL != (match = header_item_match(
                                     item, size, _transferEncoding,
                                     sizeof(_transferEncoding) - 1))) {
    *chunked = true;
  } else if (NULL != (match = header_item_match(item, size, _contentLength,
                                                sizeof(_contentLength) - 1))) {
    contentLength = atoi(match);
  }
}

bool supla_trivial_http::parse(char **in) {
  unsigned int len = strnlen((*in), INDATA_MAXSIZE);
  char next[2] = {'\r', '\n'};
  int pos = 0;

  this->resultCode = 0;
  bool chunked = false;

  for (unsigned int a = 0; a < len - sizeof(next); a++) {
    if (memcmp(&(*in)[a], next, sizeof(next)) == 0) {
      if (a - pos == 0) {
        int n = 0;
        int chunk_pos = 0;
        unsigned long chunk_left = 0;
        bool copy = !chunked;

        for (unsigned int b = a + sizeof(next); b < len; b++) {
          if (chunked) {
            if (chunk_pos == 0) {
              chunk_pos = b;
            }

            if (chunk_left > 0) {
              chunk_left--;
              if (chunk_left == 0) {
                copy = false;
                chunk_pos = b + 2;

                if (b >= len - 1 || (*in)[b] != '\r' || (*in)[b + 1] != '\n') {
                  break;
                }
              }
            }

            if (!copy && b < len - 1 && (*in)[b] == '\r' &&
                (*in)[b + 1] == '\n') {
              (*in)[b] = 0;

              if (b - chunk_pos > 0 && b - chunk_pos <= 8) {
                chunk_left = strtol(&(*in)[chunk_pos], NULL, 16);
                if (chunk_left <= 0) {
                  break;
                }
                copy = true;
              }

              b += 2;

              if (b >= len - 1) {
                break;
              }
            }
          }

          if (copy) {
            (*in)[n] = (*in)[b];
            n++;
          }
        }

        (*in)[n] = 0;
        (*in) = (char *)realloc((char *)(*in), n + 1);

        body = *in;
        *in = NULL;
        break;
      } else {
        (*in)[a] = 0;
        parse_header_item(&(*in)[pos], a - pos, &chunked);
        pos = a + sizeof(next);
      }
    }
  }

  return resultCode > 0;
}

bool supla_trivial_http::out_buffer_append(char **out_buffer, int *size,
                                           int *pos, const char *str) {
  int str_len = str ? strnlen(str, OUTDATA_MAXSIZE) : 0;
  if (str_len == 0 || (*pos) + str_len >= OUTDATA_MAXSIZE) {
    free(*out_buffer);
    *out_buffer = NULL;
    return false;
  }

  if ((*pos) + str_len + 1 > *size) {
    *size = (*pos) + str_len + 1;
    *out_buffer = (char *)realloc(*out_buffer, *size);
    if (*out_buffer == NULL) {
      return false;
    }
  }

  memcpy(&(*out_buffer)[*pos], str, str_len);
  (*pos) += str_len;
  (*out_buffer)[*pos] = 0;

  return true;
}

#define OUT_APPEND(str) \
  if (!out_buffer_append(&out_buffer, &size, &pos, str)) return false

bool supla_trivial_http::request(const char *method, const char *header,
                                 const char *data) {
  bool result = false;

  releaseResponse();

  if (!method || !host || !resource || strnlen(method, METHOD_MAXSIZE) < 3 ||
      strnlen(host, HOST_MAXSIZE) < 1 ||
      strnlen(resource, RESOURCE_MAXSIZE) < 1) {
    supla_log(LOG_ERR, "Http request - invalid parameters!");
    return false;
  }

  int pos = 0;
  int size = 100;
  char *out_buffer = (char *)malloc(size);
  memset(out_buffer, 0, size);

  OUT_APPEND(method);
  OUT_APPEND(" ");
  OUT_APPEND(resource);
  OUT_APPEND(" HTTP/1.1\r\n");
  OUT_APPEND("Host: ");
  OUT_APPEND(host);
  OUT_APPEND("\r\nUser-Agent: supla-server\r\n");

  int data_len = (data ? strnlen(data, OUTDATA_MAXSIZE) : 0);
  if (data_len > 0) {
    char number[15];
    number[0] = 0;

    snprintf(number, sizeof(number) - 1, "%i", data_len);

    OUT_APPEND("Content-Length: ");
    OUT_APPEND(number);
    OUT_APPEND("\r\n");
  }

  if (this->token) {
    OUT_APPEND("Authorization: Bearer ");
    OUT_APPEND(this->token);
    OUT_APPEND("\r\n");
  }

  OUT_APPEND("Connection: close\r\n");
  if (header) {
    OUT_APPEND(header);
    OUT_APPEND("\r\n");
  }
  OUT_APPEND("\r\n");

  if (data && data_len > 0) {
    OUT_APPEND(data);
  }

  // supla_log(LOG_DEBUG, "OUT %i [%s]", size, out_buffer);

  char *in = NULL;
  send_recv(out_buffer, &in);

  free(out_buffer);
  out_buffer = NULL;

  if (in) {
    result = parse(&in);
    if (!result) {
      supla_log(
          LOG_ERR,
          "Http request - parse error. Method: %s, resource: %s, data:\n%s",
          method, resource, in ? in : body);
    }

    if (in) {
      free(in);
      in = NULL;
    }
  } else {
    supla_log(LOG_ERR,
              "Http request - No data was received. Method: %s, resource: %s",
              method, resource);
  }

  return result;
}

void supla_trivial_http::setToken(char *token, bool copy) {
  set_string_variable(&this->token, TOKEN_MAXSIZE, token, copy);
}

bool supla_trivial_http::http_get(void) { return request("GET", NULL, NULL); }

bool supla_trivial_http::http_post(char *header, const char *data) {
  return request("POST", header, data);
}

bool supla_trivial_http::http_put(char *header, const char *data) {
  return request("PUT", header, data);
}

void supla_trivial_http::terminate(void) {
  if (sfd >= 0) {
    close(sfd);
    sfd = -1;
  }
}
