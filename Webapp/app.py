from flask import Flask, request, send_from_directory, render_template
import subprocess
import os

app = Flask(__name__)
UPLOAD_FOLDER = "uploads"
OUTPUT_FOLDER = "output"
HUFFMAN_EXE = os.path.abspath("huffman.exe")

os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(OUTPUT_FOLDER, exist_ok=True)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/compress", methods=["POST"])
def compress():
    file = request.files["file"]
    input_path = os.path.join(UPLOAD_FOLDER, "input.txt")
    output_path = os.path.join(OUTPUT_FOLDER, "compressed.txt")
    file.save(input_path)

    subprocess.run([HUFFMAN_EXE, "-c", input_path, output_path], check=True)

    return send_from_directory(OUTPUT_FOLDER, "compressed.txt", as_attachment=True)

@app.route("/decompress", methods=["POST"])
def decompress():
    file = request.files["file"]
    input_path = os.path.join(UPLOAD_FOLDER, "compressed.txt")
    output_path = os.path.join(OUTPUT_FOLDER, "decompressed.txt")
    file.save(input_path)

    subprocess.run([HUFFMAN_EXE, "-d", input_path, output_path], check=True)

    return send_from_directory(OUTPUT_FOLDER, "decompressed.txt", as_attachment=True)

if __name__ == "__main__":
    app.run(debug=True)