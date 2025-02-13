// @file scenenode.d

struct SceneNode{
		this(){
		}
    ~this(){
		}
    // Adds a child node to our current node.
    void AddChild(SceneNode* n){}
    // Draws the current SceneNode
    void Draw(){}
    // Updates the current SceneNode
    void Update(){}
    // Returns the local transformation transform
    // Remember that local is local to an object, 
		// where it's center is the origin.
    Transform GetLocalTransform(){}
    // Returns a SceneNode's world transform
    Transform GetWorldTransform(){}

private: 
    // Parent
    SceneNode* m_parent;
    // For now we have one shader per Node.
    Shader m_shader; 
    // Children holds all a pointer to all of the descendents
    // of a particular SceneNode. A pointer is used because
    // we do not want to hold or make actual copies.
    SceneNode[] m_children;
    // The object stored in the scene graph
    Object3D m_object;
    // Each SceneNode nodes locals transform.
    Transform m_localTransform;
    // We additionally can store the world transform
    Transform m_worldTransform;
}
