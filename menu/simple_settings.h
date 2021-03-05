#pragma once
#include <memory>
#include <vector>

//nama stuff
extern void simple_save( const char* path, const char* name, const void* src, size_t size, const char* filename );
extern void simple_load( const char* path, const char* name, void* dest, size_t size, const char* filename );

class ISettingNode {
public:
	virtual ~ISettingNode( ) = default;

	virtual void load( const char* path, const char* file ) = 0;
	virtual void save( const char* path, const char* file ) const = 0;
	virtual void register_( ISettingNode* node_ptr ) = 0;
};

class SettingHolder : public ISettingNode {
public:
	__forceinline SettingHolder( const char* name ) : name_( name ) {}
	__forceinline SettingHolder( SettingHolder* holder_ptr, const char* name ) : name_( name ) { holder_ptr->register_( this ); }

	void register_( ISettingNode* node_ptr ) override {
		setting_nodes_.push_back( node_ptr );
	}

	void load( const char* path, const char* file ) override {
		char full_path[ 64 ];
		strcpy_s( full_path, path );
		strcat_s( full_path, "_" );
		strcat_s( full_path, name_ );
		for ( auto x : setting_nodes_ )
			x->load( full_path, file );
	}

	void save( const char* path, const char* file ) const override
	{
		char full_path[ 64 ];
		strcpy_s( full_path, path );
		strcat_s( full_path, "_" );
		strcat_s( full_path, name_ );
		for ( auto x : setting_nodes_ )
			x->save( full_path, file );
	}

private:
	const char* name_;
	std::vector<ISettingNode*> setting_nodes_;
};

class ISetting : public ISettingNode {
	void register_( ISettingNode* node_ptr ) override {}
};

template <typename T>
class SimpleSetting : ISetting {
public:
	__forceinline SimpleSetting( SettingHolder* holder_ptr, const char* name ) : name_( name ) { holder_ptr->register_( this ); }
	__forceinline SimpleSetting( SettingHolder* holder_ptr, const char* name, const T& rhs ) : value_( rhs ), name_( name ) { holder_ptr->register_( this ); }

	void load( const char* path, const char* file ) override { simple_load( path, name_, &value_, sizeof( value_ ), file ); }

	void save( const char* path, const char* file ) const override { simple_save( path, name_, &value_, sizeof( value_ ), file ); }

	__forceinline operator T&( ) { return value_; }
	__forceinline T* operator &( ) { return &value_; }

	__forceinline T& operator ()( ) { return value_; }
private:
	T value_;
	const char* name_;
};