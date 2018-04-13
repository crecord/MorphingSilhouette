import csv

from slackclient import SlackClient
import os


compName = os.environ["COMP_NAME"]
# https://api.slack.com/custom-integrations/legacy-tokens
# This is a legacy token
# TO-DO: Make it work with Oauth token 
slack_token = "xoxp-9409267266-14198195367-312957131810-565e6b216f531d19e4e6a1d5b14be68b" 
analytics = """/home/studio/Documents/of_v20170714_linux64gcc5_release/apps/myApps/MorphingSilhouette/bin/data/analytics/""" + compName + "_analytics.csv"

comment = ""
with open(analytics, 'rb') as f:
   reader = csv.reader(f)
   cnt = 0
   for row in reader:
       cnt = cnt + 1
       comment += row[0] + ": " + row[1] + "\n "
       #comment += row[0]
       if cnt == 6:
 	   break


sc = SlackClient(slack_token)
myFile = open("/home/studio/email.jpg","rb")

sc.api_call(
  "files.upload",
  channels=["#studio-alerts"],
  title=compName,
  initial_comment= comment,
  file=myFile
)



