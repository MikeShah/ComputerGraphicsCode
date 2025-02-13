// @ file: objectmanager.d
import object3d;

/// Singleton object manager for all 3D objects
/// in our graphics application.
class ObjectManager{
	/// Retrieve an object
	static GetObject3D(size_t index){
		return mObjects[index];
	}
	/// Add a new object
	static void AddObject3D(Object3D o){
		mObjects ~= o;
	}
	/// Remove all objects
	static void RemoveAll(){
			// ...
	}
	/// Update all of the objects
	static void UpdateAll(){
		// ...
	}
	/// Render all of the objects
	static void RenderAll(){
		// ...
	}
	
	// Dynamic array to store all of our objects
	static Object3D[] mObjects;
}


void MainGameLoop(){
	// ...
	ObjectManager.GetObject3D(1).DoSomeTransformation();
	ObjectManager.GetObject3D(2).DoSomeTransformation();
	ObjectManager.GetObject3D(3).DoSomeTransformation();
	// ...
}
