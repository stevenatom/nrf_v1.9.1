#
#    Copyright (c) 2021 Project CHIP Authors
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

from typing import Optional

# Represents the node ID that is to be used when creating device
# controllers/commissioning devices
_local_node_id: Optional[int] = None

DEFAULT_LOCAL_NODE_ID = 12345


def SetLocalNodeId(node_id: int):
    """Local node id. Can be set at the start of scripts, however once set
       it cannot be reassigned.
    """
    global _local_node_id

    if _local_node_id is not None:
        raise Exception('Local node id is already set.')

    _local_node_id = node_id


def GetLocalNodeId() -> int:
    """Returns the current local node id. If none has been set, a default is set and
    used."""
    global _local_node_id

    if _local_node_id is None:
        SetLocalNodeId(DEFAULT_LOCAL_NODE_ID)

    return _local_node_id
