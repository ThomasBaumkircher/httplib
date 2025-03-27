import requests
from threading import Thread
from concurrent.futures import ThreadPoolExecutor
import time

def send_request():
    try:
        response = requests.get("http://localhost:8080")
        return response.status_code
    except requests.RequestException as e:
        return f"Error: {e}"

# Number of total requests to send
num_requests = 1000
# Number of concurrent threads to use
num_threads = 20

print(f"Sending {num_requests} requests to http://localhost:8080 using {num_threads} threads")
start_time = time.time()

with ThreadPoolExecutor(max_workers=num_threads) as executor:
    results = list(executor.map(lambda _: send_request(), range(num_requests)))

end_time = time.time()
elapsed = end_time - start_time

success_count = sum(1 for status in results if status == 200)
error_count = num_requests - success_count

print(f"Completed in {elapsed:.2f} seconds")
print(f"Successful requests: {success_count}")
print(f"Failed requests: {error_count}")
print(f"Requests per second: {num_requests/elapsed:.2f}")
