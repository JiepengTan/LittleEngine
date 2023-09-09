#include "fstream"
#include "sstream"
#include "string"
#include "FMatrix4.h"
#include "FQuaternion.h"
#include "FVector2.h"
#include "FVector3.h"
#include "FVector4.h"

class DebugUtil
{
private:
    static std::stringstream _dumpStream; 
    static float _tempMatrixData[16];
public:
    static void Flush();

    static std::string ToString();

    static void Dump(int val,bool isNewLine = false);

    static void Dump(std::string val,bool isNewLine = false);

    static void Dump(float val,bool isNewLine = false);

    static void Dump(LittleEngine::FVector2 matrix,bool isNewLine = false);

    static void Dump(LittleEngine::FVector3 matrix,bool isNewLine = false);

    static void Dump(LittleEngine::FVector4 matrix,bool isNewLine = false);

    static void Dump(LittleEngine::FQuaternion matrix,bool isNewLine = false);

    static void Dump(LittleEngine::FMatrix4 matrix,bool isNewLine = false);
};
