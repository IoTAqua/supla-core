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

#ifndef SUPLA_SCENE_REPOSITORY_H_
#define SUPLA_SCENE_REPOSITORY_H_

#include <vector>

#include "scene/scene_abstract_repository.h"
#include "scene/scene_db.h"

class supla_scene_repository : public supla_scene_abstract_repository {
 private:
 protected:
  supla_scene_db *get_db(void);

 public:
  explicit supla_scene_repository(int user_id);
  virtual ~supla_scene_repository();
  virtual supla_scene *get_scene(int id);
  virtual std::vector<supla_scene *> get_all_scenes(void);
};

#endif /* SUPLA_SCENE_REPOSITORY_H_ */
