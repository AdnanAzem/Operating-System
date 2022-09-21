
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
using namespace std;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
class guard{
    public:
    guard(){ pthread_mutex_lock(&mut);}
    ~guard(){pthread_mutex_unlock(&mut);}
};
void* tryTheGuard(void* num)
{
    guard g;
    cout<<"Not Resting"<<endl;
    sleep(4);
    cout<<"Resting"<<endl;
    cout<<num<<endl;
    return (int*)1;
}
int main(){
    pthread_t test1,test2;
    static int num = 0;
        if(pthread_create(&test2,NULL,&tryTheGuard,&num) != 0)
    {
        return 1;
    }
    if(pthread_create(&test1,NULL,&tryTheGuard,&num) != 0)
    {
        return 1;
    } 
    pthread_join(test2, NULL);
    pthread_join(test1, NULL);
}
