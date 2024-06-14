from flask import Flask, request, jsonify

app = Flask(__name__)

last_data = {}

@app.route('/', methods=['POST'])
def receive_data():
    data = request.get_json()
    if not data:
        app.logger.error("Data isn't provided")
        return jsonify({"error": "Data isn't provided"}), 400

    temp = data.get('temperature')
    humidity = data.get('humidity')

    if temp is None or humidity is None:
        app.logger.error("Invalid data: %s", data)
        return jsonify({"error": "Invalid data"}), 400

    global last_data
    last_data = data

    app.logger.info(f"Temperature: {temp} °C, humidity: {humidity} %")

    return jsonify({"message": "Data received"}), 200

@app.route('/latest', methods=['GET'])
def get_latest_data():
    return jsonify(last_data), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=2200, debug=True)