= 課題
本書で紹介したモデルでモーターが回りますが次の点から改善の余地があります。

 * 「乗り物」として見たとき危険。
 
 * より効率のよい制御に改善できる。


== デッドタイム対応
CQ EVカートのデフォルトCPU（V850ES/FG3）はモーター制御用の機能を内蔵しています。
具体的には同相のHigh側FET、Low側FETを同時にONしないようにする「デッドタイム」調整用の機能です。
たとえばU相High側FET、U相Low側FETが同時にONするタイミングです。
同相のFETがONした場合、大電流がながれFETが破壊されます。

今回Arduinoでモデルベース開発しモーター制御しましたがこのデッドタイムを考慮していません。
きちんとFETの通電タイミングを検証すべきですし、実際の製品開発ではありえないことをやっています。
デッドタイム調整機能をもたないCPUでモーター制御する場合は、
CPUとインバーター基板の間にデッドタイム調整ロジックを実装したFPGAを置く、などの対策が必要になると考えます。

もしみなさんがブラシレスモーターを3相インバーター回路で制御したい場合、デッドタイム調整をどう実現するかは重要なポイントだと考えます。


== 電源ONでモーターが急回転するリスク
本書で紹介したモデルですがアクセルを開けて電源ONするとモーターが回転します。
急回転は物理的に危険ですし、大電流が流れる可能性もあり対策が必要です。

電源ON後アクセルが開いていたら一定期間モーターを回転させないなどの安全対策が必要だと考えます。


== ホールセンサーのポーリング
CQ EVカートのデフォルトCPUはホールセンサーの立上り、立ち下がりエッジで通電パターンを設定しています。

本書のArduinoモデルベース開発の場合、ポーリングでホールセンサーのレベルを読み取っています。
ホールセンサの立上りエッジ、立ち下がりエッジを検出したときのみホールセンサーのレベルを読み込み、通電パターンを設定するのが無駄がない処理だと考えます。
私も立上り、立ち下がりエッジでのみ処理するようトライしたのですが実現できませんでした。
External Interruptという外部割り込みを実現するブロックがあり、希望の動作を実現できるかなと思いましたができませんでした。
後日ホールセンサーのポーリングを割り込み方式に改善したいと考えています。


== スロットル開度に不感帯を設ける
スロットル開度の上限、下限に不感帯を設ける、などの制御も必要かなと考えています。
スロットル開度は0〜5VですがそれをPWM設定値0〜255に割り当てています。
それを0.5V〜4.5Vの範囲をPWM設定値として割り当てる、みたいにすることが望ましいと思います。
部品の不安定な電圧部分を制御に利用しない、というのは組込み製品開発ではセオリーと感じているためです。


== 部品の保護
本書のモデルではアクセル開けた分だけモーターの回転数があがります。

アクセル開度とモーターの回転数は比例関係です。
とてもわかりやすいですが相電流を監視し部品の定格電流に到達する前にPWM制御のON幅を絞る、といった部品を保護する制御が必要だと感じています。
CQ EVカートのインバーター基板はそのような制御ができるよう追加部品を実装可能なハンダ付けスペースがあります。
より高度な制御も実現可能です。
インバーター基板の写真を再掲します。

//image[driver_pcb_extension][インバーター基板]{
//}
@<img>{driver_pcb_extension}の枠①は追加部品を実装可能なハンダ付けのスペースです。

写真には実装されていないですが過去に電流センサーを追加実装していたことがありました。そのときに必要だった抵抗、コンデンサ等が残っています。

== 機能拡張
EVカートは前述した追加部品を実装可能なハンダ付けのスペースがあり機能拡張が可能なつくりとなっています。


=== CAN通信
@<img>{driver_pcb_extension}の枠②はCANトランシーバーが実装可能なスペースです。

自動車といえばCAN通信ですがEVカートもCANトランシーバーと数点の外部部品を追加実装することでCAN通信ができます。
EVカートデフォルトのCPU基板でCAN通信は確認できました。

CAN Transmit、CAN ReceiveというブロックがあるのでSimulink + ArduinoでもCAN通信ができるようです。
ただ、CANコントローラ（MCP2515）が必要でハードウェア改造も必要です。

=== Arduino対応の拡張基板
Arduinoはラズベリーパイと並んで電子工作の定番と個人的に思っています。
Arduino対応の機能拡張基板は非常に多くあります。

今回モデルベース開発でCPUをArduinoに変更したことでいろいろな機能拡張の可能性がでてきてEVカートでできることの幅が広がったと感じています。

====[column]■開発失敗談〜FETが壊れた件〜
今回の開発中にインバーター基板のFETから火花が飛び壊れる事故がありました。
原因は@<hd>{デッドタイム対応}、@<hd>{電源ONでモーターが急回転するリスク}によるものと考えています。

火花がでたソフトウェアはSimulinkモデルではなく、Arduino C言語版のソフトウェアでした。@<br>{}
モーターに実際に通電する前にしっかりタイミングの検証をしておくべきだったと後悔しました。
モーター開発のあるあるだと思いますのでシェアさせていただきました。

====[/column]
