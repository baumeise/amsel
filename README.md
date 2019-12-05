## Amsel :bird:

This project is dedicated to develop a preferably cheap robot for freshmen students to teach them basic coding. Main focus is to learn python in a way as easy as possible without whitewash the fundamental structures of programming.<br><br>
**[Learn how it works!](https://moritzgvt.github.io/amsel/)**
<br>

### Getting Started
Read the [documentation](https://moritzgvt.github.io/amsel/getting-started) for an installation and usage guide.

### Library supported
Get `amselpy` library. [Learn more …](https://moritzgvt.github.io/amselpy/)
``` python
# import library
from amselpy import Amsel

# create instance
amsel = Amsel()

# set request adress
amsel.use("192.168.0.100")

# control movements
amsel.forward()
amsel.sleep(5)
amsel.stop()
```

### Onboard API
Use the Amsels IP address (default: `192.168.0.100`) as an endpoint to fetch data or trigger functionality.<br>
[Learn more …](https://moritzgvt.github.io/amsel/api/)

### Additional Information
This code is a result of the course "Roboter Didaktik" at University of Media Stuttgart.

### License

[MIT License](https://github.com/baumeise/amsel/blob/master/LICENSE) | Copyright © 2019 Moritz Gut (moritzgvt) 
