from google.appengine.ext import vendor
vendor.add('lib')

from flask import Flask
app = Flask(__name__)


# dynamodb = boto3.resource(
#     'dynamodb',
#     endpoint_url='http://localhost:8000',
#     region_name='dummy_region',
#     aws_access_key_id='dummy_access_key',
#     aws_secret_access_key='dummy_secret_key',
#     verify=False)

import sys

@app.route('/')
def index():
    str = "Hello, World (lets see how long a change takes III)! hello TeamID,User,NodeID,Timestamp,total_power,total_fundamental_power,reaction_power,consumed_power,sold_power\n
123,admin1,321,"2018-03-01 11:13:26",500,200,50,200,50\n
234,admin2,321,"2018-03-01 11:14:00",600,300,50,200,50\n"
#     for line in open(r"test/data.txt"):
#         str = str + line
#     open(r"test/data.txt").close()
    return str


if __name__ == '__main__':
    app.run(debug=True)
