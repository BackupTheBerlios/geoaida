#include <stdio.h>
#include <string.h>
#include <typeinfo>
class Basis {
public:
  Basis() {
    name=strdup("default Basis");
    printf("Basis() %s\n",name);
    
  }
#if 1
  Basis(const Basis &b) {
    name=b.name;
    printf("Basis(b) %s\n",name);
  }
#endif
  Basis& operator =(const Basis& b) {
    whoami();
    //    *this=b;
    printf("Basis::operator= %s\n",name);
    return *this;
  }
  virtual void whoami() {
    printf("I'm Basis\n");
  }
  virtual ~Basis() {
    printf("~Basis() %s\n",name);
  }
protected:
  char *name;
};

class Ab1 : public Basis {
public:
  Ab1() {
    name=strdup("default Ab1");
    printf("Ab1() %s\n",name);
  }
  Ab1(const Ab1 &b) {
    printf("Ab1(b) %s\n",name);   
  }
  Ab1& operator=(const Ab1& b) {
    whoami();
    //    *this=b;
    printf("Ab1::operator= %s\n",name);
    return *this;
  }
  virtual void whoami() {
    printf("I'm Ab1\n");
  }
  virtual ~Ab1() {
    printf("~Ab1() %s\n",name);
  }
};

int main()
{
   printf("============\n");
   printf("Basis b1\n");
   Basis b1;
   Basis b4;
b4 = b1;
   printf("============\n");
   printf("Ab1 a1\n");
   Ab1 a1;
   printf("============\n");
   printf("Basis b2=a1\n");
   Basis b2=a1;
   printf("============\n");
   printf("Basis b3=b2\n");
   Basis b3=b2;
   printf("============\n");
   int i=typeid(b1);
   if (typeid(b1)==typeid(int))
     printf("1\n");
   else 
     printf("0\n");
}
