# Project-Iroh

Offensive Windows security tooling that allows for persistence to the operating system. Iroh provides a variety of different persistences and misconfigurations to the system with a couple class calls. It also allows for a variety of entry points that can be built off of which will allow for the ability to mix and match persistence. This tool is currently in development and will have more features added to it as time goes on.

## DISCLAIMER

This tool is not to be used for malicious purposes and should only be used in competition or training environments.

# Project Structure

There is a static library that is compiled called `Iroh-Library` which contains the namespace of all the classes that can be used. As of right now they contain:
* IrohUser (Create persistent user)
* Firewall (Create persistent firewall rules)
* EnableServices (Enable services that are helpful for remote access)

From your prefered entrypoint you can include the `Iroh` namespace, include the headers you want to use, and add the functionality you'd like in threads. 

## Example 

``` C++
#include "../Iroh-Library/Iroh/User.h"

using namespace Iroh;

int main() {

    // Initialize the class object
    IrohUser irohUser = IrohUser();

    // Use setters to set the class variables
    irohUser.SetUsername("f1rehaz4rd");
    irohUser.SetPassword("SecurePassword-123!");
    irohUser.SetSleepTime(20000);
	
    // Start the process
    irohUser.Start();

    return 0;
}
```

You can chain multiple classes running together by threading them.

### How to use each

For how to use each class take a look at the header files for documentation.


# Special Thanks

* adamyaxley for their [Obfuscate](https://github.com/adamyaxley/Obfuscate) project.

# TODO
- [ ] Create Beaconing
- [ ] Add Command Execution
- [ ] Gather System Information
- [ ] More Advanced System Persistence for services
