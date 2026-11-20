Step 1: Start the Minikube Cluster
Open your terminal and start Minikube:

bash
minikube start --driver=docker
(Wait a minute or two for the cluster to start. You should see a message saying "Done! kubectl is now configured to use 'minikube' cluster".)

Step 2: Create the Nginx Deployment
You have two ways to do this. The easiest way for a lab is using the imperative command:

bash
kubectl create deployment nginx-deployment --image=nginx
(This pulls the official Nginx image from Docker Hub and creates 1 pod).

Step 3: Expose the Service on Port 8090
By default, Nginx listens on port 80 inside the container. We need to map host port 8090 to container port 80.

Run this command:

bash
kubectl expose deployment nginx-deployment --type=NodePort --port=80 --target-port=80 --name=nginx-service
Wait, the command above uses a random port! To specifically force port 8090, you need to use the --node-port flag if your cluster allows it, or we can create a NodePort YAML file.

Let's do it the proper way using a YAML file to guarantee port 8090:

Create a file named nginx-service.yaml:

yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx-service
spec:
  type: NodePort
  selector:
    app: nginx-deployment
  ports:
    - port: 80
      targetPort: 80
      nodePort: 30090   # Kubernetes NodePorts must be between 30000-32767
Apply it:

bash
kubectl apply -f nginx-service.yaml
⚠️ Important Note about Port 8090: Kubernetes Services (NodePort) only allow ports between 30000 and 32767. You cannot set the NodePort to 8090. However, we will use Minikube's port forwarding to make it accessible on 8090 in your browser in Step 4!

Step 4: Verify in the Browser (Using Port 8090)
Since Kubernetes blocks ports below 30000 for NodePorts, we use Minikube's port-forwarding feature to map port 8090 on your local machine to the service.

Open a new terminal window (keep the old one open) and run this command:

bash
kubectl port-forward service/nginx-service 8090:80
(Leave this terminal running! Do not close it).

Open your web browser and go to:
http://localhost:8090

You should see the "Welcome to nginx!" page. 📸 Take a screenshot for your lab record.

Step 5: Scale the Deployment to 4 Replicas
Now, scale the Nginx deployment up from 1 pod to 4 pods:

bash
kubectl scale deployment nginx-deployment --replicas=4
Step 6: List all Pods and Services
To verify everything is working, run these two commands:

1. List all Pods:

bash
kubectl get pods
Expected Output: You should see nginx-deployment-xxxx listed 4 times, all with a Running status.

2. List all Services:

bash
kubectl get services
Expected Output: You should see nginx-service with a NodePort type, showing the ports 80:30090/TCP.

📸 Take a screenshot of these terminal outputs for your lab record.

Full Command Summary (For Copy-Paste)
If you want to run everything at once, here is the quick summary:

bash
# 1. Start cluster
minikube start

# 2. Create deployment
kubectl create deployment nginx-deployment --image=nginx

# 3. Expose the service (using NodePort)
kubectl expose deployment nginx-deployment --type=NodePort --port=80 --target-port=80

# 4. Port forward to 8090 (Run this in a separate terminal)
kubectl port-forward service/nginx-deployment 8090:80

# 5. Scale to 4 replicas
kubectl scale deployment nginx-deployment --replicas=4

# 6. Verify
kubectl get pods
kubectl get svc
