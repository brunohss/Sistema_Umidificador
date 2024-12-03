# Sistema de Controle com Rele e Sensor de Umidade

Este projeto, desenvolvido por **Bruno Henrique dos Santos Silva**, utiliza um Arduino para controlar um relé com base na leitura de sensores de temperatura e umidade, além de oferecer ajustes dinâmicos por potenciômetros.

## Visão Geral

O sistema implementa um controle automatizado que:
- Lê sensores de temperatura e umidade (DHT11).
- Ajusta tempos de execução e espera através de potenciômetros.
- Exibe informações detalhadas em um display LCD 16x2 via comunicação I2C.
- Controla o acionamento de um relé com base em condições específicas.

## Componentes Utilizados

- **Microcontrolador:** Arduino
- **Sensores:**
  - DHT11 para temperatura e umidade
- **Atores:**
  - Relé para controle de dispositivos externos
- **Display:**
  - LCD 16x2 com interface I2C
- **Outros:**
  - Potenciômetros para ajustes de tempos

## Funcionamento

1. **Leitura de Sensores:** 
   - O sistema lê a umidade e a temperatura do ambiente utilizando o sensor DHT11.
2. **Ajustes Dinâmicos:**
   - Três potenciômetros controlam:
     - Tempo de execução do relé.
     - Tempo inicial de espera.
     - Multiplicador de espera em relação à umidade.
3. **Controle do Relé:**
   - O relé é acionado com base nas condições configuradas e permanece ligado pelo tempo ajustado.
4. **Exibição no LCD:**
   - O display mostra os valores atuais dos sensores, tempos de execução, espera, e status do sistema.
5. **Falhas no Sensor:**
   - Quando o sensor está desconectado ou apresenta erros, o sistema exibe uma mensagem de alerta no display.

## Código em Destaque

- **Funções Principais:**
  - `imprimir`: Exibe valores no LCD ajustando dinamicamente a posição.
  - `imprimirMi`: Converte tempo em segundos para minutos e segundos, formatando a exibição no LCD.
  - `ligar`: Controla o temporizador para manter o relé acionado.

## Configuração de Hardware

### Pinagem
| Componente       | Pino no Arduino |
|-------------------|-----------------|
| Potenciômetro 1  | A6              |
| Potenciômetro 2  | A3              |
| Potenciômetro 3  | A7              |
| Relé             | 5               |
| Sensor DHT11     | 3               |
| LCD (I2C)        | 0x27            |

### Esquema Elétrico
- Conectar os potenciômetros aos pinos A6, A3 e A7 com resistores de pull-down.
- Conectar o sensor DHT11 ao pino digital 3 com um resistor pull-up.
- Conectar o LCD 16x2 via interface I2C ao barramento de comunicação.

## Como Usar

1. Configure o hardware conforme a seção **Configuração de Hardware**.
2. Carregue o código no Arduino usando a IDE Arduino.
3. Ajuste os potenciômetros para definir os tempos desejados.
4. Observe o status no LCD e o funcionamento do relé.

## Licença

Este projeto é distribuído sob a licença MIT. Consulte o arquivo `LICENSE` para mais detalhes.

---

**Desenvolvedor:** Bruno Henrique dos Santos Silva
