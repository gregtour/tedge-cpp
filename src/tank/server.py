#!/usr/bin/python
import cgi, shelve, os, time

print "Content-Type: text/html; charset=ISO-8859-1\n"

class Server:
	MAX_TIME = 300
	def __init__(self, name, ip, time):
		self.name = name
		self.ip = ip
		self.time = time

	def checkTime(self):
		#return True
		return ((time.time() - self.time) <= Server.MAX_TIME)

	def updateTime(self):
		self.time = time.time()
			

def AddServer(name, ip, time):
	db[ip] = Server(name, ip, time)
	print "Server added '%s'" % (name)

def UpdateServer(ip):
	if db.has_key(ip):
		server = db[ip] 
		server.updateTime()
		db[ip] = server
		print "Server ping received from '%s'" % (server.name)
	else:
		AddServer("server0", ip, time.time())

# Test code: 
#class Value():
	#def __init__(self, val):
		#self.value = val
#cgi.os.environ['REMOTE_ADDR'] = "192.168.1.104"
#form = {"servername":Value("hazx")}
#form = {"reqlist":Value("")}

def main():
	form = cgi.FieldStorage()

	
	if form.has_key("servername") and form["servername"].value != "":
		if form.has_key("ping"):
			UpdateServer(cgi.os.environ['REMOTE_ADDR'])
		else:
			AddServer(form["servername"].value, cgi.os.environ['REMOTE_ADDR'], time.time())
	elif form.has_key("reqlist"):
		print "Complete server list follows:<br>"
		for ip, server in db.iteritems():
			if not server.checkTime():
				del db[ip]
			else:
				#print "'%s' running on %s for %i more seconds<br>" % (server.name, ip, (Server.MAX_TIME - time.time() + server.time))
				print "%s;%s;<br>" % (server.name, ip)
	else:
		print "Who are you and what are you doing here..."

if os.path.exists("servers.dat"):
	db = shelve.open("servers.dat", "w")
else:
	db = shelve.open("servers.dat", "c")
main()
db.close()

