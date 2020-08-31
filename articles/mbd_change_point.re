
= モデルベース開発変更点
@<hd>{what_evkart|構成}のデフォルト構成からモデルベース開発を行うために変更した点を本章に書きます。

== 構成
@<img>{diff_evkart_block}の上がモデルベース開発のブロック図です。
下はデフォルト構成のブロック図です。
//image[diff_evkart_block][モデルベース開発（上）とデフォルト構成（下）のブロック図]{
//}

=== 治具基板
モデルベース開発でもインバーター基板は変更せずに使用します。
モデルベース開発ではインバーター基板とCPU基板の間に治具基板が入ります。
次が治具基板が追加となる理由です。

 * モデルベース開発ツールに対応しているArduinoはEVカートのCPU基板とピンヘッダの形状が異なるため、インバーター基板に挿せない。

そこでArduinoを治具基板に指し、治具基板をインバーター基板に指す構造を考えました。
@<img>{gw_pcb}は治具基板の写真です。
//image[gw_pcb][治具基板]{
//}

ちょっとお見せするのが恥ずかしい出来栄えですがこれが治具基板です。
Arduinoに合体できるようにオスのピンヘッダが左右に2列あります。
また、インバーター基板と合体できるようにオスの50ピンヘッダが左右にあります。

@<img>{gw+driver_pcb}はインバータ基板と治具基板を合体させた時の写真です。
//image[gw+driver_pcb][治具基板]{
//}

@<img>{gw+driver+arduino}はインバータ基板+治具基板にArduinoを合体させた時の写真です。
//image[gw+driver+arduino][インバータ基板+治具基板+Arduino合体写真]{
//}

Arduino基板表面にあるピンヘッダを治具基板に指す形になるのでArduino基板裏面が上になります。
これでモデルベース開発をおこなうためのハードウェアの準備が完了です。


//embed[latex]{
\clearpage
//}
=== CPU
==== Arduino MEGA
デフォルトのEVカートCPU基板からArduino MEGAに変更します。

@<img>{arduino_Mega}はArduino MEGAの写真です。
//image[arduino_Mega][Arduino MEGA]{
//}

なぜArduino MEGAを選択したのか、理由は次のとおりです。

 * 無償でモデルベース開発に対応しているのがArduinoだったから。他にラズベリーパイも対応していますがEVカートの制御内容・ボリュームを考えるとArduinoの性能がベストと考えました。

 * Arduinoは安いから。安価なArduino UNOではなく、Arduino MEGAを選択した理由は後述しているシミュレーションができるから。


==== Arduino MEGAピン配置
@<img>{arduino_mega_pinout}はArduino MEGAのピン配置です。
//image[arduino_mega_pinout][Arduino MEGAピン配置]{
//}

モーター制御に使う信号は表のArduino MEGAの各ピンに割り当てています。
//table[arduino_mega_pinout_table][Arduino MEGAピン配置表]{ 
.	信号名称		Arduinoピン番号 
------------------------------------
1	ホールセンサU相	19
2	ホールセンサV相	20
3	ホールセンサW相	21
4	U相FET High側	5
5	U相FET Low側	2
6	V相FET High側	3
7	V相FET Low側	6
8	W相FET High側	7
9	W相FET Low側	8
10	スロットル		A0
11	パイロットLED	13
//}


== 開発環境
モデルベース開発は次の環境で行っています。

=== MATLAB, Simulink
MATLAB, SimulinkはMathWorks社の開発ツールです。

Simulinkはブロックと呼ばれる部品を配置・設定し、ブロック同士をつなげていくことでCPUで動くオブジェクトモジュールをつくれます。

従来のC/C++組込みソフトウェアのプログラミングに比べ、抽象度の高い記法で記述できます。

MATLABはSimulinkで開発する際に必須となるツールです。
MATLAB、Simulinkをベースに多種多様なツールが存在しますが、本書ではMATLAB、Simulinkのみを使います。
下記は今回使用したMATLAB、Simulinkのバージョンです。

//cmd{
>> ver
---------------------------------------------------------------------------------------------------
MATLAB バージョン: 9.7.0.1319299 (R2019b) Update 5
オペレーティング システム: Mac OS X  Version: 10.15.6 Build: 19G73 
Java バージョン: Java 1.8.0_202-b08 with Oracle Corporation Java HotSpot(TM) 64-Bit Server VM mixed mode
---------------------------------------------------------------------------------------------------
MATLAB          バージョン 9.7           (R2019b)
Simulink        バージョン 10.0          (R2019b)
//}

=== Simulink Support Package for Arduino Hardware 
バージョン19.2.3を使用しました。

「Simulink Support Package for Arduino Hardware」とはSimulinkがサポートしているハードウェアのパッケージソフトウェアです。
このサポートパッケージ対応しているハードウェアはSimulinkからハードウェア上で実行可能なオブジェクトファイルをつくれます。

====[column]■どうやって開発を進めたか?
今回の開発は次のステップで進めました。

 1. Arduino + C言語でモーターが回ることを確認する。
 2. Arduino + Simulinkモデルでモーターが回ることを確認する。

1の目的はハードウェアの確認です。モデルベース開発するために治具基板とArduinoを組み合わせて使うことに決めました。
治具基板は手作りなのでArduinoと組み合わせてモーターを回せるのかハードウェアの確認が必要でした。
Simulinkモデルを作りモーターが回らなかった場合、ハードウェア要因かソフトウェア要因か切り分けが難しくなるかもしれないと
考えたので前述の1→2の順番で開発を進めることにしました。

1の段階でモーターが回せていればハードウェアの不具合は疑う必要がなくなるのでモデルベース開発に専念できます。@<br>{}
単純にモデルベース開発の経験値がなく何をどうしたらよいかわからなかったことも理由です。

====[/column]
