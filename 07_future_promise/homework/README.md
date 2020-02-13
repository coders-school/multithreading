## Task1
We are trying to build new CI system. You will be responsible for implementing new notification component.  
Here are the most important attributes that **you have to achieve**:
* Performance: the component should be able to notify 5 devs in **2 seconds**.
* Debuggability: In case of error while sending mails the component should print debug info.
> Sending mail to: dev1@company.com - OK  
> Sending mail to: dev2@company.com - OK  
> Sending mail to: dev3@company.com - FAIL  
> Sending mail to: dev4@company.com - FAIL  
> Sending mail to: dev5@company.com - FAIL  


Your SW architect decided to use libmail as a framework for mail notifications.  
libmail can be found in 3rdparty folder

## Task2
libmail2.0 was released and SW architect wants you to use it.  
libmail2.0 can be found in 3rdparty folder

## Task3
SW architect wants you to improve debug info. Error reason should also be printed.
