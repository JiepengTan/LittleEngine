#include "OvMaths/DebugUtil.h"


std::stringstream DebugUtil::_dumpStream;
float DebugUtil::_tempMatrixData[16];

void DebugUtil::Flush()
{
    auto outPath = "I:/Projects/LearnOpenGL_Autho/Test.txt";
    std::ofstream outfile;
    outfile.open(outPath, std::ios_base::trunc);
    if (outfile.is_open())
    {
        outfile << _dumpStream.str() << std::endl;
    }
    outfile.close();
}

std::string DebugUtil::ToString()
{
    return _dumpStream.str();
}

void DebugUtil::Dump(int val, bool isNewLine)
{
    _dumpStream << val;
    if(isNewLine) _dumpStream<<std::endl;
}

void DebugUtil::Dump(std::string val, bool isNewLine)
{
    _dumpStream << val;
    if(isNewLine) _dumpStream<<std::endl;
}

void DebugUtil::Dump(float val, bool isNewLine)
{
    _dumpStream << std::to_string((int)(val*100));
    if(isNewLine) _dumpStream<<std::endl;
}

void DebugUtil::Dump(OvMaths::FVector2 matrix, bool isNewLine)
{
    Dump("(");
    Dump(matrix.x);
    Dump(",");
    Dump(matrix.y);
    Dump(")");
    if(isNewLine) _dumpStream<<std::endl;
}

void DebugUtil::Dump(OvMaths::FVector3 matrix, bool isNewLine)
{
    Dump("(");
    Dump(matrix.x);
    Dump(",");
    Dump(matrix.y);
    Dump(",");
    Dump(matrix.z);
    Dump(")");
    if(isNewLine) _dumpStream<<std::endl;
}

void DebugUtil::Dump(OvMaths::FVector4 matrix, bool isNewLine)
{
    Dump("(");
    Dump(matrix.x);
    Dump(",");
    Dump(matrix.y);
    Dump(",");
    Dump(matrix.z);
    Dump(",");
    Dump(matrix.w);
    Dump(")");
    if(isNewLine) _dumpStream<<std::endl;
}

void DebugUtil::Dump(OvMaths::FQuaternion matrix, bool isNewLine)
{
    Dump("(");
    Dump(matrix.x);
    Dump(",");
    Dump(matrix.y);
    Dump(",");
    Dump(matrix.z);
    Dump(",");
    Dump(matrix.w);
    Dump(")");
    if(isNewLine) _dumpStream<<std::endl;
}

void DebugUtil::Dump(OvMaths::FMatrix4 matrix, bool isNewLine)
{
    for (int i=0;i<16;i++)
    {
        auto val = std::to_string((int)(matrix.data[i]*100));
        _dumpStream << " "<<val;
    }
    _dumpStream<< " ";
    if(isNewLine) _dumpStream<<std::endl;
}
