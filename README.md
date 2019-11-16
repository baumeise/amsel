# Amsel :bird:
This project is dedicated to develop a preferably cheap robot for freshmen students to teach them basic coding. Main focus is to learn the python in a way as easy as possible without whitewash the fundamental structures of programming.

## Getting Stated
Check the [official documentation page](https://moritzgvt.github.io/amsel/) for further information.

## Usage
Get [amselpy](https://github.com/moritzgvt/amselpy) library, then:
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

## Additional Information
This Project is a result of the course "Roboter Didaktik" at University of Media Stuttgart

## License

MIT License | Copyright © 2019 Moritz Gut (moritzgvt) 
