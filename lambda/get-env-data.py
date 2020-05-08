import boto3
from boto3.dynamodb.conditions import Key, Attr
import time
import json

def lambda_handler(event, context):
    
    TABLE_NAME = "greenhouse"

    #convert to int as dynamodb doesn't like floats
    current_time = int(time.time())
    
    #create table resource
    dynamodb = boto3.resource('dynamodb', region_name="eu-west-2")
    table = dynamodb.Table(TABLE_NAME)
    
    payload = {}
    payload['temperature'] = []
    payload['humidity'] = []
    payload['illuminance'] = []
    payload['pressure'] = []
    payload['uva'] = []
    payload['uvb'] = []
    payload['uvIndex'] = []
    
    #Have to convert type as the numbers from DynamoDB get converted to Decimal objects, which aren't JSON serializable
    # Also neatens up the data to the format { type : { timestamp: value } }
    response = table.query(KeyConditionExpression=Key('type').eq('temperature') & Key('timestamp').gt(current_time - 86400))
    for i in response['Items']:
        payload['temperature'].append({int(i['timestamp']) : float(i['value'])})
    
    response = table.query(KeyConditionExpression=Key('type').eq('humidity') & Key('timestamp').gt(current_time - 86400))
    for i in response['Items']:
        payload['humidity'].append({int(i['timestamp']) : float(i['value'])})
    
    response = table.query(KeyConditionExpression=Key('type').eq('illuminance') & Key('timestamp').gt(current_time - 86400))
    for i in response['Items']:
        payload['illuminance'].append({int(i['timestamp']) : float(i['value'])})
    
    response = table.query(KeyConditionExpression=Key('type').eq('pressure') & Key('timestamp').gt(current_time - 86400))
    for i in response['Items']:
        payload['pressure'].append({int(i['timestamp']) : float(i['value'])})
    
    response = table.query(KeyConditionExpression=Key('type').eq('uva') & Key('timestamp').gt(current_time - 86400))
    for i in response['Items']:
        payload['uva'].append({int(i['timestamp']) : float(i['value'])})
        
    response = table.query(KeyConditionExpression=Key('type').eq('uvb') & Key('timestamp').gt(current_time - 86400))
    for i in response['Items']:
        payload['uvb'].append({int(i['timestamp']) : float(i['value'])})
    
    response = table.query(KeyConditionExpression=Key('type').eq('uvIndex') & Key('timestamp').gt(current_time - 86400))
    for i in response['Items']:
        payload['uvIndex'].append({int(i['timestamp']) : float(i['value'])})
    
    return {
        'statusCode': 200,
        'body': json.dumps(payload)
    }