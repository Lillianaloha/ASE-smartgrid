# Copyright 2015 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import sys
import os.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir, 'code')))

# import mysql.connector

import main
import unittest

class MainTest(unittest.TestCase):
    """This class uses the Flask tests app to run an integration test against a
    local instance of the server."""

    def setUp(self):
        self.app = main.app.test_client()

    def test_hello_world(self):
        rv = self.app.get('/')
        print(rv.data)
        assert("smartgrid1" in rv.data.lower())
    
#     def find_by_id(id):
#           cnx = mysql.connector.connect(user='root', password='lillian', host='127.0.0.1', database='ase')
#           cursor = cnx.cursor()
#           cursor.execute("SELECT * FROM smartgrid WHERE TeamID = %s",[id])
#           row = dict(zip(cursor.column_names, cursor.fetchone()))
#           cursor.close()
#           cnx.close()
#           return row


#     def createCSV(self):
#         cnx = mysql.connector.connect(user='root', password='lillian', host='127.0.0.1', database='ase')
#         cursor = cnx.cursor()
#         cursor.execute("SELECT * FROM smartgrid")

#         # Convert everything into a CSV file
#         with open('SmartGrid.txt', 'wb') as file:

#             while True:

#                 data = TLSsocket.recv(BUFFER_SIZE)
#                 if not data:
#                     break
#                 # write data to a file

#         file.write(data)
#         cursor.close()
#         cnx.close()

if __name__ == '__main__':
    unittest.main()
