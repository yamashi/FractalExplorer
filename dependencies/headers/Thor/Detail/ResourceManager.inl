/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

namespace thor
{

template <class Resource, class ResourceKey>
ResourceManager<Resource, ResourceKey>::ResourceManager()
: mMap(new ResourceMap())
, mReleaseStrategy(Resources::ExplicitRelease)
, mLoadingFailureStrategy(Resources::ThrowException)
{
}

template <class Resource, class ResourceKey>
ResourcePtr<Resource> ResourceManager<Resource, ResourceKey>::Search(const ResourceKey& key)
{
	SlotConstIterator itr = mMap->find(key);
	
	// Return found resource or NULL if not found
	if (itr == mMap->end())
		return ResourcePtr<Resource>();
	else
		return ResourcePtr<Resource>(itr->second.Share());
}

template <class Resource, class ResourceKey>
ResourcePtr<const Resource> ResourceManager<Resource, ResourceKey>::Search(const ResourceKey& key) const
{
	return const_cast<ResourceManager*>(this)->Search(key);
}

template <class Resource, class ResourceKey>
ResourcePtr<Resource> ResourceManager<Resource, ResourceKey>::Acquire(const ResourceKey& key)
{
	SlotIterator itr = mMap->find(key);

	// Load and insert resource, if not stored yet
	if (itr == mMap->end())
		return AddResource(key);

	// If resource is already stored, return pointer to it
	return ResourcePtr<Resource>(itr->second.Share());
}

template <class Resource, class ResourceKey>
bool ResourceManager<Resource, ResourceKey>::Release(const ResourceKey& key)
{
	// Find corresponding map entry
	SlotIterator itr = mMap->find(key);
	assert(itr != mMap->end());
	
	// Unload and remove resource (don't call RemoveResource() because the last shared_ptr's deleter erases the iterator)
	ResourceSlot& slot = itr->second;
	const bool unused = slot.IsUnused();

	// AFTER checking for unused, release ownership of strong-ref shared_ptr
	// This either directly releases the resource, or at the latest when external strong refs lose ownership.
	slot.KeepStrongReference(false);

	// Release() is successful if the resource is not in use
	return unused;
}

template <class Resource, class ResourceKey>
void ResourceManager<Resource, ResourceKey>::SetLoadingFailureStrategy(Resources::LoadingFailureStrategy strategy)
{
	mLoadingFailureStrategy = strategy;
}

template <class Resource, class ResourceKey>
void ResourceManager<Resource, ResourceKey>::SetReleaseStrategy(Resources::ReleaseStrategy strategy)
{
	mReleaseStrategy = strategy;
}

template <class Resource, class ResourceKey>
ResourcePtr<Resource> ResourceManager<Resource, ResourceKey>::AddResource(const ResourceKey& key)
{
	// Try to load resource, react with strategy at failure
	MovedPtr<Resource, NoCopy> resource = key.Load();
	if (!resource)
	{
		switch (mLoadingFailureStrategy)
		{
			case Resources::ThrowException:
				throw ResourceLoadingException("Failed to load resource \"" + detail::GetKeyInfo(key) + "\"");

			case Resources::ReturnNullPointer:
				return ResourcePtr<Resource>();
		}
	}

	// When succeeded, insert empty resource slot into the map
	SlotIterator itr = mMap->insert( std::make_pair(key, ResourceSlot()) ).first;

	// Create deleter able to call std::map::erase() and to release the resource
	detail::ResourceDeleter<Resource, ResourceKey> deleter(mMap, itr);

	// Post-initialize inserted slot, return shared pointer to resource
	return ResourcePtr<Resource>( itr->second.Initialize(resource.Release(), deleter, mReleaseStrategy == Resources::ExplicitRelease) );
}

template <class Resource, class ResourceKey>
void ResourceManager<Resource, ResourceKey>::RemoveResource(SlotIterator itr)
{
	// Deallocate and remove resource from map
	mMap->erase(itr);
}

} // namespace thor
