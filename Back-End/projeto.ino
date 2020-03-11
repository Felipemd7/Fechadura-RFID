#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#define SS_PIN 8 //usando a porta 8 pq a ethernet shield vai utilizar a porta 10
#define RST_PIN 9 //9
MFRC522 mfrc522(SS_PIN, RST_PIN); // criar a instancia MFRC522.
//GRANT ALL ON *.* TO 'root'@'%' IDENTIFIED BY 'root' WITH GRANT OPTION;
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int led_liberado = 2;
int led_negado = 4;
Servo microservo9g;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192,168,0,108); // ip arduino

IPAddress servidor_bd(192,168,0,100);  // IP do Servidor Mysql
char user[] = "root";              // Usuario do MySQL
char password[] = "root";        // Senha do Usuario MySQL

char cartao[16];
String cardapio ="";

// query para inserir dados
char INSERT_SQL[] = "INSERT INTO CONTROLE.tags(CARTAO,DATA)VALUES('%s',CURRENT_TIMESTAMP())";

// Buscar cartão no banco de dados
char CARTAO_SQL[] = "SELECT nome FROM CONTROLE.cadastrados where cartao='%s' ";

//Verifica o cartão
char VERIFICA_SQL[] = "SELECT cartao FROM CONTROLE.tags where cartao='%s' and data=CURDATE()";
//D2 C9 8F 1B   
// NUBANK C5F7E58D
char query[80];

EthernetClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
  microservo9g.attach(3);
 
  microservo9g.write(50);
  
  SPI.begin(); // Inicia  SPI bus
  mfrc522.PCD_Init();  // Inicia MFRC522

  Serial.begin(115200);
  lcd.begin(16,2);
    //liga 
    lcd.backlight();
    delay(250);
    //desliga
    lcd.noBacklight();
    delay(250);
    
    //finaliza com a luz de fundo ligada
    lcd.backlight(); 
  mensageminicial();
  
         Ethernet.begin(mac,ip);

         if (conn.connect(servidor_bd, 3306, user, password)) {
             lcd.clear();
            lcd.setCursor(4,0);
            lcd.print(F("CONECTOU!"));
            delay(3000);
            lcd.clear();
            } else {
              
          
           
             lcd.clear();
             lcd.setCursor(5,0);
            lcd.print(F("FALHOU"));
           delay(3000);
            lcd.clear();
         }
          
 mensageminicial();
}



void mensageminicial(){
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(F("APROXIME")); 
  lcd.setCursor(2,1);
  lcd.print(F(" SEU CARTAO")); 

 

}

void loop() {

  // busca novo cartão
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
Serial.print(F("Codigo do cartao: "));

  String conteudo= "";

  for (byte i = 0; i < mfrc522.uid.size; i++) 
     {
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
     
     }

        conteudo.toUpperCase();
        conteudo.toCharArray(cartao, 16);

        MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
        
        sprintf(query, CARTAO_SQL, cartao); //busca os dados no banco
  // Serial.println(query); //mostra o select
        cur_mem->execute(query); //executa a query informada

        column_names *cols = cur_mem->get_columns();
        for (int f = 0; f < cols->num_fields; f++) {
            Serial.print(cols->fields[f]->name); //mostra a columa da tabela
            if (f < cols->num_fields-1) {
          //     Serial.print(F(", "));
               }
        }
        
        row_values *row = NULL;
        row_values *linhas = NULL;
        int confirmado = 0;
        int inserir = 1;
        String NomeAluno;

        do {
           row = cur_mem->get_next_row();
           if (row != NULL) {

              NomeAluno = (row->values[0]);
              confirmado = 1;        
            }
    
         }while (row != NULL);

         if (confirmado == 1) {

             sprintf(query,VERIFICA_SQL, cartao);
             cur_mem->execute(query); //executa a query para verificar se ja votou hoje

             column_names *colunas = cur_mem->get_columns();
             for (int A = 0; A < colunas->num_fields; A++) {
                 Serial.print(colunas->fields[A]->name); //mostra a columa da tabela
                 if (A < colunas->num_fields-1) {
                 Serial.println(", ");
                 }
              }

             row_values *linhas = NULL;

             do{
                linhas = cur_mem->get_next_row();
                if (linhas != NULL) {
                   inserir = 0;
                  }
                  
              } while (linhas != NULL);
              
           
                sprintf(query, INSERT_SQL, cartao, 1); //gravar os dados no banco       
                cur_mem->execute(query);
                lcd.clear();
                lcd.setCursor(3,0);
                lcd.print(F("CONFIRMADO"));
                lcd.setCursor(5,1);                         
                lcd.print(NomeAluno);
                microservo9g.write(-50);
               digitalWrite(5, HIGH);
                delay(2000);
                digitalWrite(5, LOW);   
                delay(2000);
                microservo9g.write(50);
                
          
         }else {
      
           lcd.clear();
           lcd.setCursor(4,0);
           lcd.print((conteudo));
           lcd.setCursor(4,1);
           lcd.print(F("INVALIDO"));
           digitalWrite(4, HIGH);
           delay(2000);
           digitalWrite(4, LOW);           
          }
      delay(1000);
      mensageminicial(); 
     
      delete cur_mem;  

}
