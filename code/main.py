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
    str = """Hello, """
    with open("test/data.txt" , 'r') as f:
        first_line = f.readline()
        for line in f:
            return (line.split(",")[0])


if __name__ == '__main__':
    app.run(debug=True)
