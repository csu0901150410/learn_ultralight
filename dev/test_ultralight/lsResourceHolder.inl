template <typename _Resource, typename _Identifier>
inline void lsResourceHolder<_Resource, _Identifier>::load(_Identifier id, const std::string& filename) {
	std::unique_ptr<_Resource> resource(new _Resource());
    if (!resource->loadFromFile(filename)) {
        throw std::runtime_error("lsResourceHolder::load - Failed to load " + filename);
    }

    insertResource(id, std::move(resource));
}

template <typename _Resource, typename _Identifier>// for class
template <typename _Parameter>// for function
inline void lsResourceHolder<_Resource, _Identifier>::load(_Identifier id, const std::string& filename, const _Parameter& param) {
    std::unique_ptr<_Resource> resource(new _Resource());
    if (!resource->loadFromFile(filename, param)) {
        throw std::runtime_error("lsResourceHolder::load - Failed to load " + filename);
    }

    insertResource(id, std::move(resource));
}

template <typename _Resource, typename _Identifier>
inline _Resource &lsResourceHolder<_Resource, _Identifier>::get(_Identifier id) {
    auto it = m_resourceMap.find(id);
    assert(it != m_resourceMap.end());
    return *(it->second);
}

template <typename _Resource, typename _Identifier>
inline const _Resource &lsResourceHolder<_Resource, _Identifier>::get(_Identifier id) const {
    auto it = m_resourceMap.find(id);
    assert(it != m_resourceMap.end());
    return *(it->second);
}

template <typename _Resource, typename _Identifier>
inline void lsResourceHolder<_Resource, _Identifier>::insertResource(_Identifier id, std::unique_ptr<_Resource> resource) {
    auto ret = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(ret.second);
}