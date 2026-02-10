// @ file: objectmanager.d
import object3d;

/// Singleton object manager for all 3D objects
/// in our graphics application.
class ObjectManager{
	/// Retrieve an object
	static GetObject3D(size_t index){
		return mObjects[index];
	}

  // 'Factory' Method to create 3D Objects
  static Object3D Create(T...)(T params){
    Object3D result;
    /// ...
    return result;
  }

	/// Add a new object to the manager
	static void AddObject3D(Object3D o){
		mObjects ~= o;
	}
	/// Remove all objects
	static void RemoveAll(){ /* ... */ }
	/// Update all of the objects
	static void UpdateAll(){	/* ... */ }
	/// Render all of the objects
	static void RenderAll(){	/* ... */	}
	
	// Dynamic array to store all of our objects
	static Object3D[] mObjects;
}

void MainGameLoop(){

  // Create an object manually or from some configuration file
  auto newObject = ObjectManager.Create();  

  // Add the object to our manager
  ObjectManager.AddObject3D(newObject);

  // Do something interesting with our collection of objects
	// ...
	ObjectManager.GetObject3D(1).DoSomeTransformation();
	ObjectManager.GetObject3D(2).DoSomeTransformation();
	ObjectManager.GetObject3D(3).DoSomeTransformation();
	// ...
  
  // Perform some 'mutable' operation on an object.
  // That is why we have the 'ref' on 'o'
  foreach(ref o ; ObjectManager.mObjects){
     o.DoSomeTransformation();
  }

}
