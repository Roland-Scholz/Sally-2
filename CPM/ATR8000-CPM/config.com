1K* ྷ�B0���  :� �(B=(�=(�*�  -��B �G!  �!� �~ �#�08��
0�)T])) _�:�2+� ��B0� 		� ! 	>$�s"�*��y14ͫ8��02�!  "�! 	�q"�>:�s++:��Aw*��y13ͫ8��1*��w�*��q"���(*��yDSͫ8� ��D 	*����*��q"�*��y13ͫ8�*���1w:�<2�!��ډ!  "��[��>:�s�*��	F�˸͈͜���G͈͜*��	~�G͈͡:�<2�!���*��q�	� NYͫ�Y*�>$�s"�"�*��yACͫ8��A2�*��q"�*��yACͫ8��A2�*��q"�*��y19ͫ8��12�*�>:�s�:��	G͈͡�>:�s�:��G͈͜:��G͈͡*��q�	� NYͫ�Y*�*�>$�s��y�
� �>?�s�	� !� �K ���K*  �@�	 ��! ��	 ���  >?  ���^�� �~#�? ��! 	>)��s��~�.����>,�� �> ���
� �:�� :�����8��?�� �7��a��{�� �           (                                         �2                                               !K�"I��lddsysgen 0 !  �!��~ � _�!a��^ �� ~(�#�:2��J��B�8�!�5 �5  ��B8�5 �5 :ͅ!:��(.��lfreptr00 :��ͅ�lfretop00 ���ͅ�lntrksa !  "��2��l *��	~�� !�4��!R�e�怶ͅ:�<2�!��8��lratea !  "��l *��	~�ͅ:�<2�!��8�:��(�lds5in32 :�= �lonedsk01 �liiobyt :��>�(>�ͅ:�= �ltxmask0000 �libaud :��!U�e:��~8��ͅ�l :��!X�eͅ:��(@�l0 :͘:͘�l07 !�F	���l ��~#��ͅ����l �ͅ�lQ !K�"I��M(P���4h4h��L)9L�o0$~���r���[I�~� (����SI�#��͘�͘6#"I���Ɛ'�@'*I�w#"I���	����!���*I�~#"I�˿� ����/��C�y��F��/��F�*  �!@�	 ���           I�~� (����O!b	���q!v��z͒�q�. ?� * �!  9" �8" ͉�Z* " �+* �� '/7?v�����������������"*2:��EI  SPHLDI  XCHGPCHLXTHLRET HLT CMC STC CMA DAA RAR RAL RRC RLC NOP CPI ORI XRI ANI SBI IN  SUI OUT ACI ADI CALLJMP LDA STA LHLDSHLDMOV ADD ADC SUB SBB ANA XRA ORA CMP INR DCR MVI LXI STAXINX DAD LDAXDCX RST PSW POP PUSHNZZ NCC POPEP M B C D E H L M A B   D   H   SP  PSW ??= M !Eâê��ö������-Ð�f���"J��  * "�!�" !  " �2O! " "]"�"�! 1��! �++"���"M>�28 !�"9 :] � ��!  �í	1�********************************
** ATR8000 CP/M configuration **
**   program   version 1.0    **
********************************

$

Drive Configuration Menu
------------------------

1. Number of drives in system

   enter number of drives {1 thru 4} ?


2. Characteristics of drive X:

   1) 8 inch 77 track.
   2) 5 inch 40 track.
   3) 5 inch 80 track.

   enter drive type {1 thru 3} ?

   S) single sided.
   D) double sided.

   enter head type {S or D} ?

   1) fast step.
   2) medium step.
   3) slow step.
   
   enter step rate {1 thru 3} ?


   drive settings
   --------------   
A: not configured                              .
B: not configured                              .
C: not configured                              .
D: not configured                              .
   
   are these correct {Y/N} ?$




Printer Configuration Menu
--------------------------

1. Type of printer

   A) parallel Centronics.
   B) serial without handshake.
   C) serial with DTR handshake.

   enter printer type {A thru C} ?



RS232 Port Configuration Menu
-----------------------------

1. Parity and wordlength

   A) 8 bits with no parity.
   B) 7 bits with odd parity.
   C) 7 bits with even parity.

   enter parity/wordlength {A thru C} ?


2. Baudrate

   1) 9600 baud.
   2) 4800 baud.
   3) 2400 baud.
   4) 1200 baud.
   5)  600 baud.
   6)  300 baud.
   7)  150 baud.
   8)  110 baud.
   9)   75 baud.

   enter baudrate (1 thru 9) ?


         character device settings
         -------------------------
PRINTER:                                      .
  RS232:                                      .

         are these correct {Y/N} ?$




Auto Command Execution on Boot-Up
---------------------------------

   enter command line to be executed
when CP/M is initially loaded (or
press return to omit).

?


CONFIG done, chaining to DDSYSGEN ... 
$
