#include "merkletree.h"
#include "twisted-edwards/Point.h"
#include "nlohmann/json.hpp"
#include "primitives/transaction.h"
#include "supervisor/supervisor.h"
/**
 * 1. 首先，不知道这个input应该是由谁产生
 * 2. 码片数据，公共参数这些，不知道以何形式存在
 * 3. 
 */
void generateInput(CTransaction &tx,uint64_t compIndex)
{
    
}   