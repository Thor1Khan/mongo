/*    Copyright 2012 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "mongo/pch.h"

#include "mongo/db/auth/principal_set.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <vector>

#include "mongo/db/auth/principal.h"
#include "mongo/util/mongoutils/str.h"

namespace mongo {

    PrincipalSet::PrincipalSet() {}
    PrincipalSet::~PrincipalSet() {
        for (std::vector<Principal*>::iterator it = _principals.begin();
                it != _principals.end(); ++it) {
            delete *it;
        }
    }

    void PrincipalSet::add(Principal* principal) {
        for (std::vector<Principal*>::iterator it = _principals.begin();
                it != _principals.end(); ++it) {
            Principal* current = *it;
            if (current->getDBName() == principal->getDBName()) {
                // There can be only one principal per database.
                delete current;
                _principals.erase(it);
                break;
            }
        }
        _principals.push_back(principal);
    }

    void PrincipalSet::removeByDBName(const std::string& dbname) {
        for (std::vector<Principal*>::iterator it = _principals.begin();
                it != _principals.end(); ++it) {
            Principal* current = *it;
            if (current->getDBName() == dbname) {
                delete current;
                _principals.erase(it);
                break;
            }
        }
    }

    Principal* PrincipalSet::lookup(const std::string& name, const std::string& dbname) const {
        Principal* principal = lookupByDBName(dbname);
        if (principal && principal->getName() == name) {
            return principal;
        }
        return NULL;
    }

    Principal* PrincipalSet::lookupByDBName(const std::string& dbname) const {
        for (std::vector<Principal*>::const_iterator it = _principals.begin();
                it != _principals.end(); ++it) {
            Principal* current = *it;
            if (current->getDBName() == dbname) {
                return current;
            }
        }
        return NULL;
    }

} // namespace mongo
