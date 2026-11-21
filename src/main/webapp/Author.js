Step 1: Launch the EC2 Instance (AWS Console)
Log in to the AWS Console (or Learner Lab).

Go to EC2 → Click Launch Instance.

Name: trainrepo-deploy

AMI: Select Ubuntu 22.04 LTS.

Instance Type: Select t2.micro (Free tier).

Key Pair: Select vockey (or create a new one and download the .pem file).

Network Settings (Security Group): Click Edit and add these 3 rules (Make sure the IP Version is IPv4 for all of them!):

Type: SSH | Port: 22 | Source: 0.0.0.0/0 (or My IP)

Type: HTTP | Port: 80 | Source: 0.0.0.0/0

Type: Custom TCP | Port: 8040 | Source: 0.0.0.0/0

Click Launch Instance.

Go to the EC2 Dashboard, select your instance, and copy the Public IPv4 address (e.g., 13.222.164.33).

💻 Step 2: Connect via SSH (From your Laptop)
Open Git Bash (or your terminal).

bash
# Go to the folder where your .pem key is downloaded
cd ~/Downloads

# Set read-only permission for the key
chmod 400 labsuser.pem

# Connect to the EC2 instance (Replace with your actual Public IP)
ssh -i labsuser.pem ubuntu@<YOUR_EC2_PUBLIC_IP>
(Type yes if it asks for fingerprint confirmation).

🐳 Step 3: Install Docker & Git (Inside EC2 Terminal)
Once you see the ubuntu@ip-xxx-xxx:~$ prompt, run these commands:

bash
# 1. Update package list
sudo apt update -y

# 2. Install Docker and Git
sudo apt install -y docker.io git

# 3. Start and enable Docker service
sudo systemctl start docker
sudo systemctl enable docker

# 4. Grant permissions to Ubuntu user to run Docker without 'sudo'
sudo usermod -aG docker ubuntu

# 5. Apply the group changes immediately
newgrp docker
📥 Step 4: Clone the Repository
bash
git clone https://github.com/anujyog1/trainrepo.git
cd trainrepo
(Run ls to confirm you see pom.xml, src, etc.)

📄 Step 5: Create the Dockerfile
We will use the "Direct JSP Deploy" method (No Maven needed).

Open the file:

bash
nano Dockerfile
Paste this exact code:

dockerfile
FROM tomcat:9.0-jdk11-temurin

# Remove default Tomcat apps
RUN rm -rf /usr/local/tomcat/webapps/*

# Copy the webapp folder directly to Tomcat's ROOT directory
COPY src/main/webapp/ /usr/local/tomcat/webapps/ROOT/

EXPOSE 8080
CMD ["catalina.sh", "run"]
Save and exit: Press Ctrl + O, hit Enter, then Ctrl + X.

Step 6: Build and Run the Container
bash
# 1. Build the Docker image
docker build -t trainrepo-app .

# 2. Run the container (Mapping host port 8040 to container port 8080)
docker run -d --name train-container -p 8040:8080 trainrepo-app

# 3. Verify the container is running
docker ps
 Step 7: Verify in Browser
Open your web browser on your laptop.

Go to: http://<YOUR_EC2_PUBLIC_IP>:8040
(Example: http://13.222.164.33:8040)

You should see the deployed Java Web App!
