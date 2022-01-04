/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <CubismFramework.hpp>
#include <Utils/CubismString.hpp>

 /**
  * @brief wavファイルハンドラ
  * @attention 16bit wav ファイル�iみ�zみのみ�g廾�gみ
  */
class WavFileHandler
{
public:
    /**
     * @brief コンストラクタ
     */
    WavFileHandler();

    /**
     * @brief デストラクタ
     */
    ~WavFileHandler();

    /**
     * @brief wavファイルハンドラの坪何彜�B厚仟
     *
     * @param[in]   deltaTimeSeconds    デルタ�r�g[昼]
     * @retval  true    厚仟されている
     * @retval  false   厚仟されていない
     */
    Csm::csmBool Update(Csm::csmFloat32 deltaTimeSeconds);

    /**
     * @brief 哈方で峺協したwavファイルの�iみ�zみを�_兵する
     *
     * @param[in] filePath wavファイルのパス
     */
    void Start(const Csm::csmString& filePath);

    /**
     * @brief �F壓のRMS�ﾈゝ�
     *
     * @retval  csmFloat32 RMS��
     */
    Csm::csmFloat32 GetRms() const;

private:
    /**
     * @brief wavファイルのロ�`ド
     *
     * @param[in] filePath wavファイルのパス
     * @retval  true    �iみ�zみ撹孔
     * @retval  false   �iみ�zみ払��
     */
    Csm::csmBool LoadWavFile(const Csm::csmString& filePath);

    /**
     * @brief PCMデ�`タの盾慧
     */
    void ReleasePcmData();

    /**
     * @brief -1゛1の���譴�1サンプル函誼
     * @retval    csmFloat32    屎�ｻ�されたサンプル
     */
    Csm::csmFloat32 GetPcmSample();

    /**
     * @brief �iみ�zんだwavfileの秤��
     */
    struct WavFileInfo
    {
        /**
         * @brief コンストラクタ
         */
        WavFileInfo() : _fileName(""), _numberOfChannels(0),
            _bitsPerSample(0), _samplingRate(0), _samplesPerChannel(0)
        { }

        Csm::csmString _fileName; ///< ファイル兆
        Csm::csmUint32 _numberOfChannels; ///< チャンネル方
        Csm::csmUint32 _bitsPerSample; ///< サンプルあたりビット方
        Csm::csmUint32 _samplingRate; ///< サンプリングレ�`ト
        Csm::csmUint32 _samplesPerChannel; ///< 1チャンネルあたり�tサンプル方
    } _wavFileInfo;

    /**
     * @brief バイトリ�`ダ
     */
    struct ByteReader {
        /**
         * @brief コンストラクタ
         */
        ByteReader() : _fileByte(NULL), _fileSize(0), _readOffset(0)
        { }

        /**
         * @brief 8ビット�iみ�zみ
         * @return Csm::csmUint8 �iみ函った8ビット��
         */
        Csm::csmUint8 Get8()
        {
            const Csm::csmUint8 ret = _fileByte[_readOffset];
            _readOffset++;
            return ret;
        }

        /**
         * @brief 16ビット�iみ�zみ�┘螢肇襯┘鵐妊�アン��
         * @return Csm::csmUint16 �iみ函った16ビット��
         */
        Csm::csmUint16 Get16LittleEndian()
        {
            const Csm::csmUint16 ret = (_fileByte[_readOffset + 1] << 8) | _fileByte[_readOffset];
            _readOffset += 2;
            return ret;
        }

        /**
         * @brief 24ビット�iみ�zみ�┘螢肇襯┘鵐妊�アン��
         * @return Csm::csmUint32 �iみ函った24ビット�｣�和了24ビットに�O協��
         */
        Csm::csmUint32 Get24LittleEndian()
        {
            const Csm::csmUint32 ret =
                (_fileByte[_readOffset + 2] << 16) | (_fileByte[_readOffset + 1] << 8)
                | _fileByte[_readOffset];
            _readOffset += 3;
            return ret;
        }

        /**
         * @brief 32ビット�iみ�zみ�┘螢肇襯┘鵐妊�アン��
         * @return Csm::csmUint32 �iみ函った32ビット��
         */
        Csm::csmUint32 Get32LittleEndian()
        {
            const Csm::csmUint32 ret =
                (_fileByte[_readOffset + 3] << 24) | (_fileByte[_readOffset + 2] << 16)
                | (_fileByte[_readOffset + 1] << 8) | _fileByte[_readOffset];
            _readOffset += 4;
            return ret;
        }

        /**
         * @brief シグネチャの函誼と歌孚猟忖双との匯崑チェック
         * @param[in] reference �������鵑離轡哀優船穃鍔崛�
         * @retval  true    匯崑している
         * @retval  false   匯崑していない
         */
        Csm::csmBool GetCheckSignature(const Csm::csmString& reference)
        {
            Csm::csmChar getSignature[4] = { 0, 0, 0, 0 };
            const Csm::csmChar* referenceString = reference.GetRawString();
            if (reference.GetLength() != 4)
            {
                return false;
            }
            for (Csm::csmUint32 signatureOffset = 0; signatureOffset < 4; signatureOffset++)
            {
                getSignature[signatureOffset] = static_cast<Csm::csmChar>(Get8());
            }
            return (getSignature[0] == referenceString[0]) && (getSignature[1] == referenceString[1])
                && (getSignature[2] == referenceString[2]) && (getSignature[3] == referenceString[3]);
        }

        Csm::csmByte* _fileByte; ///< ロ�`ドしたファイルのバイト双
        Csm::csmSizeInt _fileSize; ///< ファイルサイズ
        Csm::csmUint32 _readOffset; ///< ファイル歌孚了崔
    } _byteReader;

    Csm::csmFloat32** _pcmData; ///< -1から1の���譴捻蹶Fされた咄蕗デ�`タ塘双
    Csm::csmUint32 _sampleOffset; ///< サンプル歌孚了崔
    Csm::csmFloat32 _lastRms; ///< 恷瘁に���yしたRMS��
    Csm::csmFloat32 _userTimeSeconds; ///< デルタ�r�gの�e麻��[昼]
 };
