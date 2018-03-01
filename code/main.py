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

def printRes():
    #     return "123 234"
    f1 = open("data.txt","w+")
    f1.write("TeamID,User,NodeID,Timestamp,total_power,total_fundamental_power,reaction_power,consumed_power,sold_power\n")
    f1.write("123,admin1,321,'2018-03-01 11:13:26',500,200,50,200,50\n")
    f1.write("234,admin2,321,'2018-03-01 11:14:00',600,300,50,200,50\n")
    f1.close()
    res = " "
    with open("data.txt" , 'r+') as f:
        first_line = f.readline()
        for line in f:
            res = res + line.split(",")[0] + " "
    return res

@app.route('/')
def index():
    return "123 234"

if __name__ == '__main__':
    app.run(debug=True)
