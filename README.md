Our team is currently working to build a website allowing communication to the power grid, so that we can implement certain simulations such as price regulation, demand response and power distribution, use a database to save progress and maybe support multiple users connecting at the same time.

What we have completed are showing as follows:
### Set up a CI/CD pipeline and develop our own test cases [![CircleCI]
(https://circleci.com/gh/Lillianaloha/test.svg?style=svg)](https://circleci.com/gh/Lillianaloha/test)

Following the github instruction, we successfully apply Circle CI testing into our project. And by testing, we better grab what we should do, and fix our codes more practicable.

### Build a data generator
Our data generator is built on the basis of a testbed with 20 nodes connecting to smart power meters, which is to simulate a real world power grid. Receiving time and sampling rate from the webpage input, the data generator could generate data based on a timer, and send data back to the webpage and database.

### Build a user interface for researchers and educators
Our website enables users to input time and sampling rate, and get generated data. The plots of current-time and voltage-time would show on the webpage. Moreover, it allows users to download csv files to save data to the local.
The development of our website promises to make smart grid learning experiences more accessible, more personal, and more relevant. Educators can use the testbed and engage the students as an ideal socially constructed process. For researchers, the web application helps the development of the next generation of the electic utility grid —— performing research, creating innovations, demonstrating advanced wireless communications, and using internet and sense-and-control technologies.

### Build SQL database
The MySQL database is for data storage and retrieval. It can be updated real-time for users to check the data for the future use.

### Front-end and back-end integration
We use sockets to connect the front-end webpage and the C codes (data generator). It is a really challenging part in our project, that requires PHP socket and C socket. The webpage enables users to input parameters, and the socket pass data between these two sides.

### RSA file verification system using java
This is another function we add to the project that allows RSA file verification.

------------------------------------------------------------------------------------------------------------------------------------------
We have completed the project using 3 Phase physics equations, but a component we wanted to add but proved to be more challenging than expected (especially with finals season) is to use techniques of machine learning to generate new Smart Meter readings based on previous real readings (acquired from few publicly available datasets). 
One of the papers that showed the most promise has been attached to this repository. This is to show the project proved to be more intense than I expected.

------------------------------------------------------------------------------------------------------------------------------------------
To better use our product, there is brief description of each folder in our project repository. 'generator' is for data generator which is built by C code. 'verify' is for java RSA file verification system, 'front-end' is for web application design, in which 'plot' is for current-time and voltage-time plot displaying, 'html' includes all the main interfaces, 'css' is for website style, and you can just ignore 'others' subfolder because it is just for our simple test. In 'plot' subfolder, 'socket.php' is for socket connection between front-end and back-end parts, and it will automatically jump to 'index_graph.php' (the plot page) when data is passing back. When you click on the button in the plot page, it will jump to a new page 'table.php' in 'html' subfolder which displays the newest 12 data saved in the MySQL database. 
