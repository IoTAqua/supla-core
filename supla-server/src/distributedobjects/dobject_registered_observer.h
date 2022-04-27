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

#ifndef SUPLA_DISTRIBUTED_OBJECT_REGISTERED_OBSERVER_H_
#define SUPLA_DISTRIBUTED_OBJECT_REGISTERED_OBSERVER_H_

#include <list>

#include "distributedobjects/dobject_change_indicator.h"
#include "distributedobjects/dobject_observer.h"

class supla_dobject_registered_observer {
 private:
  std::list<supla_dobject_change_indicator *> change_indicators;
  supla_dobject_observer *observer;

 protected:
 public:
  explicit supla_dobject_registered_observer(supla_dobject_observer *observer);
  virtual ~supla_dobject_registered_observer();
  const supla_dobject_observer *get_observer(void);
};

#endif /* SUPLA_DISTRIBUTED_OBJECT_REGISTERED_OBSERVER_H_ */
