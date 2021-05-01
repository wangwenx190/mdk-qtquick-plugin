/*
 * MIT License
 *
 * Copyright (C) 2021 by wangwenx190 (Yuhang Zhao)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "mdkplayer_global.h"
#include <QtCore/qurl.h>
#include <QtCore/qtimer.h>
#include <QtQuick/qquickitem.h>

namespace mdk
{

class Player;

}

MDKPLAYER_BEGIN_NAMESPACE

class VideoTextureNode;

class MDKPLAYER_API MDKPlayer : public QQuickItem
{
    Q_OBJECT
#ifdef QML_ELEMENT
    QML_ELEMENT
#endif
    Q_DISABLE_COPY_MOVE(MDKPlayer)
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QSizeF videoSize READ videoSize NOTIFY videoSizeChanged)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool mute READ mute WRITE setMute NOTIFY muteChanged)
    Q_PROPERTY(bool seekable READ seekable NOTIFY seekableChanged)
    Q_PROPERTY(PlaybackState playbackState READ playbackState WRITE setPlaybackState NOTIFY playbackStateChanged)
    Q_PROPERTY(MediaStatus mediaStatus READ mediaStatus NOTIFY mediaStatusChanged)
    Q_PROPERTY(LogLevel logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)
    Q_PROPERTY(qreal playbackRate READ playbackRate WRITE setPlaybackRate NOTIFY playbackRateChanged)
    Q_PROPERTY(qreal aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged)
    Q_PROPERTY(QString snapshotDirectory READ snapshotDirectory WRITE setSnapshotDirectory NOTIFY snapshotDirectoryChanged)
    Q_PROPERTY(QString snapshotFormat READ snapshotFormat WRITE setSnapshotFormat NOTIFY snapshotFormatChanged)
    Q_PROPERTY(QString snapshotTemplate READ snapshotTemplate WRITE setSnapshotTemplate NOTIFY snapshotTemplateChanged)
    Q_PROPERTY(QStringList videoSuffixes READ videoSuffixes CONSTANT)
    Q_PROPERTY(QStringList audioSuffixes READ audioSuffixes CONSTANT)
    Q_PROPERTY(QStringList subtitleSuffixes READ subtitleSuffixes CONSTANT)
    Q_PROPERTY(QStringList videoMimeTypes READ videoMimeTypes CONSTANT)
    Q_PROPERTY(QStringList audioMimeTypes READ audioMimeTypes CONSTANT)
    Q_PROPERTY(QString positionText READ positionText NOTIFY positionTextChanged)
    Q_PROPERTY(QString durationText READ durationText NOTIFY durationTextChanged)
    Q_PROPERTY(bool hardwareDecoding READ hardwareDecoding WRITE setHardwareDecoding NOTIFY hardwareDecodingChanged)
    Q_PROPERTY(QStringList videoDecoders READ videoDecoders WRITE setVideoDecoders NOTIFY videoDecodersChanged)
    Q_PROPERTY(QStringList audioDecoders READ audioDecoders WRITE setAudioDecoders NOTIFY audioDecodersChanged)
    Q_PROPERTY(QStringList defaultVideoDecoders READ defaultVideoDecoders CONSTANT)
    Q_PROPERTY(QStringList defaultAudioDecoders READ defaultAudioDecoders CONSTANT)
    Q_PROPERTY(QStringList audioBackends READ audioBackends WRITE setAudioBackends NOTIFY audioBackendsChanged)
    Q_PROPERTY(bool autoStart READ autoStart WRITE setAutoStart NOTIFY autoStartChanged)
    Q_PROPERTY(bool livePreview READ livePreview WRITE setLivePreview NOTIFY livePreviewChanged)
    Q_PROPERTY(FillMode fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)
    Q_PROPERTY(MediaInfo mediaInfo READ mediaInfo NOTIFY mediaInfoChanged)
    Q_PROPERTY(bool loop READ loop WRITE setLoop NOTIFY loopChanged)

    friend class VideoTextureNode;

public:
    enum class PlaybackState
    {
        Stopped,
        Playing,
        Paused
    };
    Q_ENUM(PlaybackState)

    enum class MediaStatus
    {
        Unknown,
        NoMedia,
        Unloaded,
        Loading,
        Loaded,
        Prepared,
        Stalled,
        Buffering,
        Buffered,
        End,
        Seeking,
        Invalid
    };
    Q_ENUM(MediaStatus)

    enum class LogLevel
    {
        Off,
        Info,
        Debug,
        Warning,
        Critical,
        Fatal
    };
    Q_ENUM(LogLevel)

    struct ChapterInfo
    {
        qint64 beginTime = 0;
        qint64 endTime = 0;
        QString title = {};
    };
    using Chapters = QList<ChapterInfo>;

    using MetaData = QHash<QString, QString>;

    enum class FillMode
    {
        PreserveAspectFit,
        PreserveAspectCrop,
        Stretch
    };
    Q_ENUM(FillMode)

    struct VideoStreamInfo
    {
        int index = 0;
        qint64 startTime = 0;
        qint64 duration = 0;
        QString codec = {};
        qint64 bitRate = 0;
        qreal frameRate = 0.0;
        QString format = {};
        int width = 0;
        int height = 0;
        MetaData metaData = {};
    };
    using VideoStreams = QList<VideoStreamInfo>;

    struct AudioStreamInfo
    {
        int index = 0;
        qint64 startTime = 0;
        qint64 duration = 0;
        QString codec = {};
        qint64 bitRate = 0;
        qreal frameRate = 0.0;
        int channels = 0;
        int sampleRate = 0;
        MetaData metaData = {};
    };
    using AudioStreams = QList<AudioStreamInfo>;

    struct MediaInfo
    {
        qint64 startTime = 0;
        qint64 duration = 0;
        qint64 bitRate = 0;
        qint64 fileSize = 0;
        QString format = {};
        int streamCount = 0;
        Chapters chapters = {};
        MetaData metaData = {};
        VideoStreams videoStreams = {};
        AudioStreams audioStreams = {};
    };

    explicit MDKPlayer(QQuickItem *parent = nullptr);
    ~MDKPlayer() override;

    QUrl source() const;
    void setSource(const QUrl &value);

    QString fileName() const;

    QString filePath() const;

    qint64 position() const;
    void setPosition(const qint64 value);

    qint64 duration() const;

    QSizeF videoSize() const;

    qreal volume() const;
    void setVolume(const qreal value);

    bool mute() const;
    void setMute(const bool value);

    bool seekable() const;

    PlaybackState playbackState() const;
    void setPlaybackState(const PlaybackState value);

    MediaStatus mediaStatus() const;

    LogLevel logLevel() const;
    void setLogLevel(const LogLevel value);

    qreal playbackRate() const;
    void setPlaybackRate(const qreal value);

    qreal aspectRatio() const;
    void setAspectRatio(const qreal value);

    QString snapshotDirectory() const;
    void setSnapshotDirectory(const QString &value);

    QString snapshotFormat() const;
    void setSnapshotFormat(const QString &value);

    QString snapshotTemplate() const;
    void setSnapshotTemplate(const QString &value);

    static inline QStringList videoSuffixes()
    {
        static const QStringList list =
        {
            QStringLiteral("*.3g2"),   QStringLiteral("*.3ga"),
            QStringLiteral("*.3gp"),   QStringLiteral("*.3gp2"),
            QStringLiteral("*.3gpp"),  QStringLiteral("*.amv"),
            QStringLiteral("*.asf"),   QStringLiteral("*.asx"),
            QStringLiteral("*.avf"),   QStringLiteral("*.avi"),
            QStringLiteral("*.bdm"),   QStringLiteral("*.bdmv"),
            QStringLiteral("*.bik"),   QStringLiteral("*.clpi"),
            QStringLiteral("*.cpi"),   QStringLiteral("*.dat"),
            QStringLiteral("*.divx"),  QStringLiteral("*.drc"),
            QStringLiteral("*.dv"),    QStringLiteral("*.dvr-ms"),
            QStringLiteral("*.f4v"),   QStringLiteral("*.flv"),
            QStringLiteral("*.gvi"),   QStringLiteral("*.gxf"),
            QStringLiteral("*.hdmov"), QStringLiteral("*.hlv"),
            QStringLiteral("*.iso"),   QStringLiteral("*.letv"),
            QStringLiteral("*.lrv"),   QStringLiteral("*.m1v"),
            QStringLiteral("*.m2p"),   QStringLiteral("*.m2t"),
            QStringLiteral("*.m2ts"),  QStringLiteral("*.m2v"),
            QStringLiteral("*.m3u"),   QStringLiteral("*.m3u8"),
            QStringLiteral("*.m4v"),   QStringLiteral("*.mkv"),
            QStringLiteral("*.moov"),  QStringLiteral("*.mov"),
            QStringLiteral("*.mp2"),   QStringLiteral("*.mp2v"),
            QStringLiteral("*.mp4"),   QStringLiteral("*.mp4v"),
            QStringLiteral("*.mpe"),   QStringLiteral("*.mpeg"),
            QStringLiteral("*.mpeg1"), QStringLiteral("*.mpeg2"),
            QStringLiteral("*.mpeg4"), QStringLiteral("*.mpg"),
            QStringLiteral("*.mpl"),   QStringLiteral("*.mpls"),
            QStringLiteral("*.mpv"),   QStringLiteral("*.mpv2"),
            QStringLiteral("*.mqv"),   QStringLiteral("*.mts"),
            QStringLiteral("*.mtv"),   QStringLiteral("*.mxf"),
            QStringLiteral("*.mxg"),   QStringLiteral("*.nsv"),
            QStringLiteral("*.nuv"),   QStringLiteral("*.ogm"),
            QStringLiteral("*.ogv"),   QStringLiteral("*.ogx"),
            QStringLiteral("*.ps"),    QStringLiteral("*.qt"),
            QStringLiteral("*.qtvr"),  QStringLiteral("*.ram"),
            QStringLiteral("*.rec"),   QStringLiteral("*.rm"),
            QStringLiteral("*.rmj"),   QStringLiteral("*.rmm"),
            QStringLiteral("*.rms"),   QStringLiteral("*.rmvb"),
            QStringLiteral("*.rmx"),   QStringLiteral("*.rp"),
            QStringLiteral("*.rpl"),   QStringLiteral("*.rv"),
            QStringLiteral("*.rvx"),   QStringLiteral("*.thp"),
            QStringLiteral("*.tod"),   QStringLiteral("*.tp"),
            QStringLiteral("*.trp"),   QStringLiteral("*.ts"),
            QStringLiteral("*.tts"),   QStringLiteral("*.txd"),
            QStringLiteral("*.vcd"),   QStringLiteral("*.vdr"),
            QStringLiteral("*.vob"),   QStringLiteral("*.vp8"),
            QStringLiteral("*.vro"),   QStringLiteral("*.webm"),
            QStringLiteral("*.wm"),    QStringLiteral("*.wmv"),
            QStringLiteral("*.wtv"),   QStringLiteral("*.xesc"),
            QStringLiteral("*.xspf")
        };
        return list;
    }

    static inline QStringList audioSuffixes()
    {
        static const QStringList list =
        {
            QStringLiteral("*.mp3"),
            QStringLiteral("*.aac"),
            QStringLiteral("*.mka"),
            QStringLiteral("*.dts"),
            QStringLiteral("*.flac"),
            QStringLiteral("*.ogg"),
            QStringLiteral("*.m4a"),
            QStringLiteral("*.ac3"),
            QStringLiteral("*.opus"),
            QStringLiteral("*.wav"),
            QStringLiteral("*.wv")
        };
        return list;
    }

    static inline QStringList subtitleSuffixes()
    {
        static const QStringList list =
        {
            QStringLiteral("*.utf"),
            QStringLiteral("*.utf8"),
            QStringLiteral("*.utf-8"),
            QStringLiteral("*.idx"),
            QStringLiteral("*.sub"),
            QStringLiteral("*.srt"),
            QStringLiteral("*.rt"),
            QStringLiteral("*.ssa"),
            QStringLiteral("*.ass"),
            QStringLiteral("*.mks"),
            QStringLiteral("*.vtt"),
            QStringLiteral("*.sup"),
            QStringLiteral("*.scc"),
            QStringLiteral("*.smi")
        };
        return list;
    }

    static QStringList videoMimeTypes();

    static QStringList audioMimeTypes();

    QString positionText() const;

    QString durationText() const;

    bool hardwareDecoding() const;
    void setHardwareDecoding(const bool value);

    QStringList videoDecoders() const;
    void setVideoDecoders(const QStringList &value);

    QStringList audioDecoders() const;
    void setAudioDecoders(const QStringList &value);

    // The order is important. Only FFmpeg is software decoding.
    static inline QStringList defaultVideoDecoders()
    {
#ifdef Q_OS_WINDOWS
        static const QStringList list =
        {
            QStringLiteral("MFT:d3d=11"),
            QStringLiteral("MFT:d3d=9"),
            QStringLiteral("MFT"),
            QStringLiteral("D3D11"),
            QStringLiteral("DXVA"),
            QStringLiteral("CUDA"),
            QStringLiteral("NVDEC"),
            QStringLiteral("FFmpeg")
        };
#elif defined(Q_OS_LINUX)
        static const QStringList list =
        {
            QStringLiteral("VAAPI"),
            QStringLiteral("VDPAU"),
            QStringLiteral("CUDA"),
            QStringLiteral("NVDEC"),
            QStringLiteral("FFmpeg")
        };
#elif defined(Q_OS_DARWIN)
        static const QStringList list =
        {
            QStringLiteral("VT"),
            QStringLiteral("VideoToolbox"),
            QStringLiteral("FFmpeg")
        };
#elif defined(Q_OS_ANDROID)
        static const QStringList list =
        {
            QStringLiteral("AMediaCodec"),
            QStringLiteral("FFmpeg")
        };
#else
#error "Unsupported platform!"
#endif
        return list;
    }

    static inline QStringList defaultAudioDecoders()
    {
        return {};
    }

    QStringList audioBackends() const;
    // Available audio backends: XAudio2 (Windows only), ALSA (Linux only),
    // AudioQueue (Apple only), OpenSL (Android only), OpenAL
    void setAudioBackends(const QStringList &value);

    bool autoStart() const;
    void setAutoStart(const bool value);

    bool livePreview() const;
    void setLivePreview(const bool value);

    FillMode fillMode() const;
    void setFillMode(const FillMode value);

    MediaInfo mediaInfo() const;

    bool loop() const;
    void setLoop(const bool value);

public Q_SLOTS:
    void open(const QUrl &value);
    void play();
    void play(const QUrl &value);
    void pause();
    void stop();
    // Key frame seeking is the fastest seeking but it's not accurate, for example,
    // if you want to jump to the 235 frame, the player may end up at the 248 frame
    // because the 248 frame is the nearest key frame.
    // To avoid this inaccuracy, you should not use key frame seeking, however,
    // accurate seeking will take more time because the player needs to decode the
    // media to get the image data to show (if you are not jumping to a key frame).
    void seek(const qint64 value, const bool keyFrame = true);
    // Avoid naming conflicts of QQuickItem's own functions.
    void rotateImage(const int value);
    void scaleImage(const qreal x, const qreal y);
    void snapshot();
    bool isLoaded() const;
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;
    void startRecording(const QUrl &value, const QString &format = {});
    void stopRecording();
    void seekBackward(const int value = 5000);
    void seekForward(const int value = 5000);

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#else
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#endif

private Q_SLOTS:
    void invalidateSceneGraph();

private:
    void releaseResources() override;
    void initMdkHandlers();
    void resetInternalData();

Q_SIGNALS:
    void loaded();
    void playing();
    void paused();
    void stopped();
    void sourceChanged();
    void fileNameChanged();
    void filePathChanged();
    void positionChanged();
    void durationChanged();
    void videoSizeChanged();
    void volumeChanged();
    void muteChanged();
    void seekableChanged();
    void playbackStateChanged();
    void mediaStatusChanged();
    void logLevelChanged();
    void playbackRateChanged();
    void aspectRatioChanged();
    void snapshotDirectoryChanged();
    void snapshotFormatChanged();
    void snapshotTemplateChanged();
    void positionTextChanged();
    void durationTextChanged();
    void hardwareDecodingChanged();
    void videoDecodersChanged();
    void audioDecodersChanged();
    void audioBackendsChanged();
    void autoStartChanged();
    void livePreviewChanged();
    void fillModeChanged();
    void mediaInfoChanged();
    void loopChanged();
    void newHistory(const QUrl &url, const qint64 pos);

private:
    VideoTextureNode *m_node = nullptr;

    QTimer m_timer;

    QSharedPointer<mdk::Player> m_player;

    qreal m_volume = 1.0;

    bool m_mute = false;
    bool m_hasVideo = false;
    bool m_hasAudio = false;
    bool m_hasSubtitle = false;
    bool m_hasChapters = false;
    bool m_hardwareDecoding = false;
    bool m_autoStart = true;
    bool m_livePreview = false;
    bool m_loop = false;

    QString m_snapshotDirectory = {};
    QString m_snapshotFormat = QStringLiteral("png");
    QString m_snapshotTemplate = QStringLiteral("${filename}_${datetime}_${frametime}");

    QStringList m_videoDecoders = {};
    QStringList m_audioDecoders = {};
    QStringList m_audioBackends = {};

    FillMode m_fillMode = FillMode::PreserveAspectFit;
    MediaInfo m_mediaInfo = {};
    int m_mediaStatus = 0;
};

MDKPLAYER_END_NAMESPACE

Q_DECLARE_METATYPE(MDKPLAYER_PREPEND_NAMESPACE(MDKPlayer)::ChapterInfo)
Q_DECLARE_METATYPE(MDKPLAYER_PREPEND_NAMESPACE(MDKPlayer)::Chapters)
Q_DECLARE_METATYPE(MDKPLAYER_PREPEND_NAMESPACE(MDKPlayer)::MetaData)
Q_DECLARE_METATYPE(MDKPLAYER_PREPEND_NAMESPACE(MDKPlayer)::VideoStreamInfo)
Q_DECLARE_METATYPE(MDKPLAYER_PREPEND_NAMESPACE(MDKPlayer)::VideoStreams)
Q_DECLARE_METATYPE(MDKPLAYER_PREPEND_NAMESPACE(MDKPlayer)::AudioStreamInfo)
Q_DECLARE_METATYPE(MDKPLAYER_PREPEND_NAMESPACE(MDKPlayer)::AudioStreams)
Q_DECLARE_METATYPE(MDKPLAYER_PREPEND_NAMESPACE(MDKPlayer)::MediaInfo)
