// Note: Full class shown for brevity to fit on slide                       
//       You should create separate .hpp and .cpp to                        
//       separate interface from implementation                             
#ifndef VEC3_H                                                              
#define VEC3_H                                                              
                                                                             
class vec3 {                                                                
     public:                                                                 
         // Default Constructor                                              
         vec3() : e{0,0,0} {}                                                
         // Constructor which initializes elements                           
         vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}            
         // Return each component                                            
         double x() const { return e[0]; }                                   
         double y() const { return e[1]; }                                   
         double z() const { return e[2]; }                                   
                                                                             
         // ... Other member functions ...                                   
     public:                                                                 
        double e[3];                                                        
};                                                                          
                                                                             
#endif                                                                      
                                                                            
  
