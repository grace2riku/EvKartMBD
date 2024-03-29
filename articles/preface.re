= 前書き
本書はCQ出版社から販売されているCQ EVカートのハードウェア・ソフトウェアを改造し、モデルベース開発でブラシレスモーター制御を行う方法を説明します。
私の自己学習・実験で得た知見を情報共有しています。

「EVカートで始めるモデルベース開発」という書名ですが私はモデルベース開発を業務で担当したことはありません
（補助的な立ち位置で検証業務は経験があります）。
私の技術バックボーンはC言語による組込みソフトウェア開発です。
そんな技術バックボーンをもつ私がモデルベース開発の自己学習を所有していたEVカートを利用して始めました。

制御基板はArduino MEGA 2560でモデリングツールにMATLAB、Simulinkを使用します。MATLABのライセンスは個人用途のMATLAB Homeライセンスです。
自己学習した結果、モデルベース開発でArduinoの周辺機能を制御し、EVカートのブラシレスモーターを回すことができました。
その開発過程で得た知見をまとめたのが本書です。具体的につぎの知見が学べます。

 * MATLAB、SimulinkでArduinoのペリフェラルを制御する方法@<br>{}
	GPIO入力・出力、AD変換、PWM、タイマー
 * C言語の処理をモデルベースで実現する方法

私と同じ「組込みソフトウェアエンジニアでモデルベース開発に興味を持っている方」に有益な情報となると考えています。
Simulinkモデルの説明ではC言語を併記し「C言語の記述はSimulinkではこのように書く」ということがわかりやすくなるよう工夫しました。
本書がモデルベース開発を始めたい方に役立てば嬉しいです。


== 免責事項

本書に記載された内容は、情報の提供のみを目的としています。したがって、本書を用いた開発、製作、運用は、必ずご自身の責任と判断によって行ってください。
これらの情報による開発、製作、運用の結果について、著者はいかなる責任も負いません。
