#include "../include/Common.hpp"
#include <sstream>
#include <iomanip>

bool resIdStr( const mpc::nsdk::SResId &id, std::string &strId)
{
    std::stringstream ss;
    for( size_t i = 0; i < sizeof( id ); ++i )
    {
        ss <<  std::hex << std::setw(2) <<std::setfill('0') << (uint32_t)(id.id[i]);
    }
    ss >> strId;

    return true;
}

bool str2ResId( const std::string &strId, mpc::nsdk::SResId &resId )
{
    if( strId.size() < 32 )
    {
        return false;
    }
    std::string tmp;
    for( size_t i = 0; i < strId.size(); ++i )
    {
        if( strId[i] >= '0' && strId[i] <= '9' )
        {
            tmp += strId[i];
        }
        else if( strId[i] >= 'a' && strId[i] <= 'f' )
        {
            tmp += strId[i];
        }
        else if( strId[i] >= 'A' && strId[i] <= 'F' )
        {
            tmp += strId[i];
        }
    }
    if( tmp.size() != 32 )
    {
        return false;
    }
    std::stringstream ss;
    for( size_t i = 0; i < 16; ++i )
    {
        std::string item = tmp.substr( i*2, 2 );
        uint32_t itemValue;// = atoi( item.c_str() );
        ss << std::hex << item;
        ss >> itemValue;
        resId.id[i] = itemValue;
        ss.clear();
    }
    return true;
}

void printResId( const mpc::nsdk::SResId &id )
{
    std::cout << " ";
    std::stringstream ss;
    for( size_t i = 0; i < sizeof( id ); ++i )
    {
        ss <<  std::hex << std::setw(2) <<std::setfill('0') << (uint32_t)(id.id[i]);
    }
    std::cout << ss.str();
}