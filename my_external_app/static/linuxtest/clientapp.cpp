#include <iostream>
#include <dlfcn.h>

int main() {
    using std::cout;
    using std::cerr;

    cout << "C++ dlopen demo\n\n";

    // open the library
    cout << "Opening wrapper.so...\n";
    void* handle = dlopen("./wrapper.so", RTLD_LAZY);
    
    if (!handle) {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    
    // load the symbol
    cout << "Loading symbol wrap_print_one_to_ten...\n";
    typedef void (*importfunc_t)();

    // reset errors
    dlerror();
    importfunc_t func_print1 = (importfunc_t) dlsym(handle, "wrap_print_one_to_ten");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol 'wrap_print_one_to_ten'': " << dlsym_error <<
            '\n';
        dlclose(handle);
        return 1;
    }else{
    
        // use it to do the calculation
        cout << "Calling wrap_print_one_to_ten...\n";
        //interface_print1(); //calling the loaded function using the name of the function
        func_print1(); //calling the loaded function by the function pointer
        
    
        // close the library
        cout << "Closing library...\n";
        dlclose(handle);
   }
}
