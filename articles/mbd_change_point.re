
= モデルベース開発変更点
@<hd>{what_evkart|構成}のデフォルト構成からモデルベース開発を行うために変更した点を次に書きます。

== 構成

=== 治具基板
モデルベース開発でもインバーター基板は変更せずに使用します。

モデルベース開発ではインバーター基板とCPU基板の間に治具基板が入ります。

次が治具基板が追加となる理由です。

 * モデルベース開発ツールに対応しているArduinoはEVカートのCPU基板とピンヘッダの形状が異なるため、インバーター基板に挿せない。

　　そこでArduinoを治具基板に指し、治具基板をインバーター基板に指す構造を考えた。


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


=== Arduino MEGA
デフォルトのEVカートCPU基板からArduino MEGAに変更します。

@<img>{arduino_Mega}はArduino MEGAの写真です。
//image[arduino_Mega][Arduino MEGA]{
//}

なぜArduino MEGAを選択したのか、理由は次のとおりです。

 * 無償でモデルベース開発に対応しているのがArduinoだったから。
 　
 　他にラズベリーパイも対応しているがEVカートの制御内容・ボリュームを考えるとArduinoの性能で充分と感じたから。

 * Arduinoシリーズで安価なUNOではなく、MEGAを選択した理由は後述しているシミュレーションができるから。


== 開発環境
モデルベース開発は次の環境で行っています。

=== MATLAB, Simulink
MATLAB, SimulinkはMathWorks社の開発ツールです。

Simulinkはブロックと呼ばれる部品を配置・設定し、ブロック同士をつなげていくことでCPUで動くオブジェクトモジュールをつくります。

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
バージョン19.2.3

「Simulink Support Package for Arduino Hardware」とはSimulinkがサポートしているハードウェアのことです。

このサポートパッケージ対応しているハードウェアはSimulinkがハードウェアのオブジェクトファイルをつくれます。

