Our team is currently working to build a website allowing communication to the power grid, so that we can implement certain simulations such as price regulation, demand response and power distribution, use a database to save progress and maybe support multiple users connecting at the same time.


### Current Step: set up Setup a CI/CD pipeline and develop our own trivial test case [![CircleCI](https://circleci.com/gh/Lillianaloha/test.svg?style=svg)](https://circleci.com/gh/Lillianaloha/test)

Followed the github instruction, we succesffully get the webpage which showes "Hello world!...". For the trivial test, we first trying to create a simple database, and then test if some entries are exist in the database. However, we couldn't figure out how to let tester read data from the csv file. Then, we try to change the code so that we could create a .txt file and then test if some string exist, but when we try to put the function in the decorator, the website gives a 500 interval server error. We talked with CA Sriharsha and he suggested us try to print a simple header for now. So right now our trivial test could only print the webpage with text "The TeamIDs are smartgrid1, smartgrid2". Finally, we add the badges to our readme.

--------------------------------------------------------------------------------------------------------------------------------
We have completed the project using 3 Phase physics equations, But a component we wanted to add but proved to be more challenging than expect (especially with finals season) is to use techniques of machine learning to generate new Smart Meter readings based on previous readings. One of the papers that showed most promised has been attached to this repository. This is to show the project proved to be more involved than I expected.
