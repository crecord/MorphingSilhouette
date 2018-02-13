

from slackclient import SlackClient
import os

# https://api.slack.com/custom-integrations/legacy-tokens
# This is a legacy token
# TO-DO: Make it work with Oauth token 
slack_token = "xoxp-9409267266-14198195367-312957131810-565e6b216f531d19e4e6a1d5b14be68b" 

compName = os.environ["COMP_NAME"]
sc = SlackClient(slack_token)
myFile = open("/home/studio/email.jpg","rb")

sc.api_call(
  "files.upload",
  channels=["#studio-alerts"],
  title=compName,
  file=myFile
)

