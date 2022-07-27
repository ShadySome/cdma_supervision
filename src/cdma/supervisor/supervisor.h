#include "Point.h"
#include "random.h"
#include "merkletree.h"
#include "zcash/Zcash.h"
#include <map>
#include <vector>
using namespace std;
static const int N = 32;
struct PublicParameter
{
    uint256 a;
    std::vector<NTL::Point> g;
    std::vector<NTL::Point> h;
};
class Supervisor
{
private:
    /* data */
    uint64_t num_of_comp = N;
    map<int,vector<int>> companyInfo;
    vector<vector<int>> code_matrix;
    struct PublicParameter pp;

public:
    Supervisor()= default;

    void generateCode();

    PublicParameter generatePP();

    std::vector<NTL::Point> calcCodeEccForm();

    void generateKCode();
    
};
