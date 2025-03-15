#pragma once

#include <string>
#include <memory>
#include <map>
#include <cassert>

template <typename _Resource, typename _Identifier>
class lsResourceHolder
{
public:
    void load(_Identifier id, const std::string& filename);

    template <typename _Parameter>
    void load(_Identifier id, const std::string& filename, const _Parameter& param);

    _Resource& get(_Identifier id);
    const _Resource& get(_Identifier id) const;

private:
    void insertResource(_Identifier id, std::unique_ptr<_Resource> resource);

private:
    std::map<_Identifier, std::unique_ptr<_Resource>> m_resourceMap;
};

#include "lsResourceHolder.inl"
