from django.shortcuts import render
from django.http import HttpResponse
import requests
import time
channel_id = '3017167'
url = f'https://api.thingspeak.com/channels/{channel_id}/feeds.json?results=1'

def ths():
    data = requests.get(url).json()
    feed = data['feeds'][0]
    print("Field 1:", feed['field1'], " | Field 2:", feed['field2'], " | Field 3:", feed['field3'])
    time.sleep(5)
    a=feed['field1']
    b=feed['field2']
    c=feed['field3']
    return a,b,c
def crop(request):
    a=None
    b=None
    c=None
    a,b,c=ths()
    if request.method == 'POST':
        try:
            Temperature = float(request.POST.get('Temperature', 0))
            Humidity = float(request.POST.get('Humidity', 0))
            Soilmoisture = float(request.POST.get('Soilmoisture', 0))

            prediction = (Temperature + Humidity + Soilmoisture) / 3
            print(f'Predicted Crop Yield: {prediction}')
            #a,b,c=ths()
            return HttpResponse(f'Predicted Crop Yield: {prediction:.2f}')
            #a,b,c=ths()
        except (ValueError, TypeError):
            return HttpResponse('Invalid input values. Please enter numeric values for all fields.')
        #a,b,c=ths()
    
    return render(request, 'index.html',{'a':a,'b':b,'c':c})  # Change 'crop_form.html' to your actual template name
