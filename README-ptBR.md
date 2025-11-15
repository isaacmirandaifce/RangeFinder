# Trena óptica baseada no Laser-m01-esp32
Integração de um módulo de medição de distância a laser de alta precisão em placas de desenvolvimento **ESP32**.

Fork do repositório de [Andres-ros](https://github.com/Andres-ros/laser-m01-esp32)

## Visão Geral
Este repositório apresenta um exemplo mínimo e funcional para leitura de distâncias via protocolo **UART** utilizando:

- **Módulo Laser Range M01 (placa de 6 pinos)**
- **Uma placa de desenvolvimento baseado em ESP32**

Funções disponíveis pelo Monitor Serial:  
- **Q** → Medir  
- **L** → Ligar o laser  
- **K** → Desligar o laser  
- **R** → Resetar o módulo  

---

## Segurança

* O laser trabalha em **infravermelho (≈905–940 nm)**.
* Não aponte para os olhos ou superfícies reflexivas a curta distância.

---
## Materiais Necessários
- **Módulo Laser Ranging M01 – 50 m** (Liancheng Electronics – AliExpress)  
- **Qualquer modelo ESP32 é compatível)**
- **Alimentação 3.3 V** (mínimo recomendado: 150 mA)  
- **Cabos Dupont ou solda**

---

## Pinout do Módulo (de cima para baixo)

1. **MIN (3V3)** – Alimentação 3.3 V  
2. **ENA** – Enable (ativo em nível alto)  
3. —  
4. **GND** – Terra  
5. **RXD** – Entrada UART do módulo  
6. **TXD** – Saída UART do módulo  
7. **NC** – Não conectado  
8. —

### Cores do cabo (conforme fabricante)
- **Vermelho** → MIN (3V3)  
- **Preto** → GND  
- **Verde** → RXD (módulo)  
- **Amarelo** → TXD (módulo)

**Atenção:**  
- TX do módulo → RX do ESP32  
- RX do módulo ← TX do ESP32  

(Cruzamento obrigatório)

---

## Conexão Recomendada (ESP32)
| Função | Módulo M01 | ESP32 |
|--------|-------------|----------------|
| Alimentação | Vermelho (MIN) | 3V3 |
| Terra | Preto (GND) | GND |
| TXD (módulo) | Amarelo | GPIO33 (RX) |
| RXD (módulo) | Verde | GPIO32 (TX) |
| ENA | ENA | GPIO5 HIGH ou direto ao 3V3 |

Se o seu cabo possui apenas 4 fios (vermelho/preto/verde/amarelo), normalmente o **ENA já vem interno ligado ao alto**, se não, então não é necessário conectá-lo.

---

## Uso
1. Envie o sketch para o ESP32 e abra o **Monitor Serial em 115200 baud**.  
2. Aponte o módulo para uma superfície fosca (2–4 m), preferencialmente com fundo escuro.  
3. Use as teclas no terminal:
   - **L** → Liga o laser  
   - **Q** → Medição rápida (retorna `X.XXX`)  
   - **K** → Desliga o laser  
   - **R** → Reinicia o módulo (ciclo de ENA)

**Observação:**  
O emissor é **infravermelho**, portanto o ponto não é visível a olho nu.  
Com a câmera do celular geralmente é possível visualizar.

---

## Diagnóstico Rápido (se não houver dados na RX)
- Verifique o cruzamento:  
  - Amarelo (TXD módulo) → RX ESP32  
  - Verde (RXD módulo) ← TX ESP32  
- Certifique-se de que **ambos compartilham o GND**.  
- Confirme que **ENA está em nível alto** (GPIO5 HIGH ou 3V3).  
- **Teste de eco rápido:**  
  - Jumpeie verde ↔ amarelo no conector do módulo.  
  - Tudo que o ESP32 enviar deve retornar idêntico.  
  - Se não houver eco → revisar soldas e continuidade entre pinos e o conector.  

- Algumas unidades do módulo Laser vêm configuradas para **115200 baud**.  
  Se necessário, altere:  

```cpp
  LZR.begin(9600, …)
```

para:

```cpp
LZR.begin(115200, …)
```

---

## Agradecimentos

Ao autor[Andres-ros](https://github.com/Andres-ros/laser-m01-esp32).

Agradecimento especial à **Liancheng Electronics (Shenzhen) Co., Ltd. Store**, que forneceu o esquema de cores do cabo e auxiliou na solução dos problemas de conexão RX/TX.

**Link de compra:**
[https://www.aliexpress.com/store/1104805174?spm=a2g0o.order_list.order_list_main.8.21ef194dfoSHNu](https://www.aliexpress.com/store/1104805174?spm=a2g0o.order_list.order_list_main.8.21ef194dfoSHNu)

