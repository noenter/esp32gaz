void SMTP_Init(){
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  smtpData.setSender("ESP32 Bosch6000gaz", emailSenderAccount);    // Подпись, от кого
  smtpData.setPriority("High");   // Приоритет
  smtpData.setSubject(emailSubject);
  smtpData.setMessage("Warning ESP32 ReLoad", false);  // Тут текст письма
  smtpData.addRecipient(emailRecipient);
  smtpData.setSendCallback(sendCallback);
if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
//Clear all data from Email object to free memory
//  smtpData.empty();
}

void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}

void sendCallback(SendStatus info);
