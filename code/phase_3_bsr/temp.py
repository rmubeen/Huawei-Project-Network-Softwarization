import matplotlib.pyplot as plt
import json

def main():
    names = []
    for i in range(24):
        with open(str(i), 'r') as json_file:
            json_obj = json.load(json_file)
            count = 0
            for j in json_obj["data"]:
#                if j["road_name"] not in data:
#                    data[j["road_name"]] = []
#                data[j["road_name"]].append((json_obj["date_time"], j["avg_speed_capped"]))
                if j["free_flow_speed"] not in names:
                    names.append(j["free_flow_speed"])
#                data[names[count]].append((json_obj["date_time"], j["avg_speed_capped"]))
            json_file.close()
    print names
#    print json.dumps(data, indent=4)
#plt.plot([1,2,3,4])
#plt.ylabel('some numbers')
#plt.show()

main()
