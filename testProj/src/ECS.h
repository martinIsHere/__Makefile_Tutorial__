#pragma once
#include "includes.h"
#include <vector>
#include <bitset>
#include <array>
#include <stdexcept>

constexpr int MAX_COMPONENTS = 16;
constexpr int MAX_ENTITIES = 16;


// static utility function


// useless

class Entity;
class Manager;

class Component {
public:
	Entity* ent;

	virtual void update() {

	}

	virtual void draw() {

	}

	virtual void init() {

	}

	virtual ~Component() {

	}

};

static int getEntityID() {
	static int lastID = 0;
	return lastID++;

}

class Entity {

	std::array<Component*, MAX_COMPONENTS> m_ComponentArray;
	std::bitset<MAX_COMPONENTS> m_ComponentBitset;
	bool active;
	uint32_t entityID;
public:

	//init
	Entity() { 
		m_ComponentArray = {}; 
		m_ComponentBitset = {};
		active = true;
		entityID = getEntityID();
	}

	~Entity() = default;


	static int getComponentTypeID() {
		static int lastID = 0;
		return lastID++;

	}
	// will return id of certain datatype
	// if datatype has not been entered add it

	template<typename T>
	static int getComponentTypeID() {
		static int typeID = getComponentTypeID();
		return typeID;

	}

	template <typename T, typename... tArgs> 
	T& addComponent(const tArgs&... args) {

		// creates a new component
		T* c = new T(args...);

		if (!c) {
			std::cout << "Failed in creating new component!\n";
		}

		// sets the component's instance of an entity to this one
		c->ent = this;

		// adds component to component array and activates it by setting active to true
		if (m_ComponentArray[getComponentTypeID<T>()] != nullptr) {
			
			std::cout << "Component already added!\n";
		}
		m_ComponentArray[getComponentTypeID<T>()] = c;
		m_ComponentBitset[getComponentTypeID<T>()] = true;

		return *c;

	}

	template <typename T> 
	void removeComponent() {
		// deletes component and deactivates it
		delete m_ComponentArray[getComponentTypeID<T>()];
		m_ComponentBitset[getComponentTypeID<T>()] = false;

	}

	bool isActive() const {
		return active;
	}

	void setActive() {
		active = true;
	}

	void setInactive() {
		active = false;
	}

	void destroy() {
		active = false;
	}

	template <typename T>
	bool hasComponent() const {
		// checks if given component has been added to the component array
		if (getComponentTypeID<T>() < MAX_COMPONENTS && &m_ComponentArray != nullptr) {
			if (m_ComponentArray[getComponentTypeID<T>()] != nullptr) {
				return true;
			}
		}
		return false;
		
	}

	template <typename T>
	T& getComponent() const {
		auto ptr = m_ComponentArray[getComponentTypeID<T>()];
		assertN(ptr, nullptr);
		return *(T*)ptr;

	}
	
	void update() {
		// updates all components from component array
		for (auto c : m_ComponentArray) {
				if (c) {
					c->update();
				}
		}

	}

	void draw() {
		// draws all components from component array
		for (auto c : m_ComponentArray) {
			if (c) {
				c->draw();
			}
		}

	}

	void init() {
		// inits all components from component array
		for (auto c : m_ComponentArray) {
			if (c) {
				c->init();
			}
		}

	}

	void clear() {
		// clears all components from component array
		for (auto c : m_ComponentArray) {
			if (c) {
				delete c; 
			}
		}
	
	}

};



class Manager {

	std::vector<Entity*> m_EntityArray;

public:

	Manager() = default;

	size_t getSize() {
		return m_EntityArray.size();
	}

	std::vector<Entity*>* getEntityArrayPointer() {
		return &m_EntityArray;
	}

	void update() {
		// iterate through entity array and update entities
		for (Entity* ent : m_EntityArray) {
			if (ent->isActive())
				ent->update();
		}
	}

	void draw() {
		// iterate through entity array and draw entities
		for (Entity* ent : m_EntityArray) {
			if(ent->isActive())
				ent->draw();
		}
	}

	void init() {
		// iterate through entity array and init entities
		for (auto ent : m_EntityArray) {
			ent->init();
		}
	}

	void clean() {

		// iterates through entity array and checks if entity is active

		for (int x = 0; x < m_EntityArray.size(); x++) {
			if (!m_EntityArray[x]->isActive()) {

				// if entity isn't active, delete that entity and remove from entity array
				m_EntityArray[x]->clear();
				delete m_EntityArray[x];
				std::vector<Entity*>::iterator iter = m_EntityArray.begin();
				std::advance(iter, x);
				m_EntityArray.erase(iter);
				x--;
			}
		}
	}

	void clear() {
		for (auto ent : m_EntityArray) {
			ent->clear();
			delete ent;
		}
	}

	Entity* addEntity() {
		Entity* ent = new Entity();
		m_EntityArray.emplace_back(ent);
		return  ent;
	}

	~Manager() {
		clear();
	}

};
