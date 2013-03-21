<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ru_RU">
<context>
    <name>MuscleAlignmentDialog</name>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="20"/>
        <source>Align with MUSCLE</source>
        <translation>Выравнивание MUSCLE</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="36"/>
        <source>Input file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="46"/>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="63"/>
        <source>...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="53"/>
        <source>Output file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="83"/>
        <source>Mode</source>
        <translation>Конфигурация</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="95"/>
        <source>Mode details:</source>
        <translation>Описание:</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="115"/>
        <source>Advanced options</source>
        <translation>Дополнительные настройки</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="138"/>
        <source>Max iterations</source>
        <translation>Максимальное число итераций</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="181"/>
        <source>Max time (minutes)</source>
        <translation>Максимальное время работы (мин.)</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="225"/>
        <source>Translating alignment to amino allows to avoid errors of inserting gaps within codon boundaries.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="228"/>
        <source>Translate to amino when aligning</source>
        <translation>Транслировать в амины в процессе выравнивания</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="240"/>
        <source>Translation table:</source>
        <translation>Таблица трансляций:</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="261"/>
        <source>Region to align</source>
        <translation>Выравнивать:</translation>
    </message>
    <message>
        <source>Custom range</source>
        <translation type="obsolete">Указанный регион</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="351"/>
        <source>-</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="269"/>
        <source>Whole alignment</source>
        <translation>Целые последовательности</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="284"/>
        <source>Column range</source>
        <translation>Указанный регион столбцов</translation>
    </message>
    <message>
        <source>Remote run...</source>
        <translation type="obsolete">Удаленный запуск...</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="383"/>
        <source>Align</source>
        <translation>Выровнять</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="393"/>
        <source>Cancel</source>
        <translation>Отменить</translation>
    </message>
    <message>
        <location filename="../src/ui/MuscleAlignDialog.ui" line="123"/>
        <source>Do not re-arrange sequences (-stable)</source>
        <translation>Сохранять порядок последовательностей (ключ -stable)</translation>
    </message>
</context>
<context>
    <name>U2::LocalWorkflow::MusclePrompter</name>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="124"/>
        <source> from %1</source>
        <translation> из %1</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="132"/>
        <source>Aligns each MSA supplied &lt;u&gt;%1&lt;/u&gt; with MUSCLE using &quot;&lt;u&gt;%2&lt;/u&gt;&quot; mode.</source>
        <translation>Для каждого выравнивания&lt;u&gt;%1&lt;/u&gt;, запустить MUSCLE в  &lt;u&gt;&quot;%2&quot;&lt;/u&gt; и выдать результат на  выход.</translation>
    </message>
</context>
<context>
    <name>U2::LocalWorkflow::MuscleWorker</name>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="62"/>
        <source>Input MSA</source>
        <translation>Входное выравнивание</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="64"/>
        <source>Multiple sequence alignment</source>
        <translation>Множественное выравнивание</translation>
    </message>
    <message>
        <source>MUSCLE alignment</source>
        <translation type="obsolete">Выравнивание MUSCLE</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="73"/>
        <source>Mode</source>
        <translation>Конфигурация</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="76"/>
        <source>Stable order</source>
        <translation>Сохранять порядок</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="63"/>
        <source>Multiple sequence alignment to be processed.</source>
        <translation>Входные данные для выравнивания.</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="58"/>
        <source>Region should be set as &apos;start..end&apos;, start should be less than end, e.g. &apos;1..100&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="64"/>
        <source>Result of alignment.</source>
        <translation>Результат выравнивания.</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="80"/>
        <source>Max iterations</source>
        <translation type="unfinished">Максимальное число итераций</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="81"/>
        <source>Maximum number of iterations</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="82"/>
        <source>Region to align</source>
        <translation type="unfinished">Выравнивать:</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="83"/>
        <source>Whole alignment or column range e.g. &lt;b&gt;1..100&lt;/b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="90"/>
        <source>Align with MUSCLE</source>
        <translation>Выравнивание MUSCLE</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="91"/>
        <source>MUSCLE is public domain multiple alignment software for protein and nucleotide sequences.&lt;p&gt;&lt;dfn&gt;MUSCLE stands for MUltiple Sequence Comparison by Log-Expectation.&lt;/dfn&gt;&lt;/p&gt;</source>
        <translation>Пакет MUSCLE предназначен для выравнивания множественных протеиновых и нуклеотидных последовательностей.</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="175"/>
        <source>An empty MSA has been supplied to MUSCLE.</source>
        <translation>MUSCLE получил пустое МВ.</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="199"/>
        <source>Region end position should be greater than start position</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="226"/>
        <source>Aligned %1 with MUSCLE</source>
        <translation>Выровнял %1 с MUSCLE</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="74"/>
        <source>Selector of preset configurations, that give you the choice of optimizing accuracy, speed, or some compromise between the two. The default favors accuracy.</source>
        <translation>Выбор между режимами максимальной точности, скорости либо компромиса между ними. По умолчанию оптимизируется точность выравнивания.</translation>
    </message>
    <message>
        <location filename="../src/MuscleWorker.cpp" line="77"/>
        <source>Do not rearrange aligned sequences (-stable switch of MUSCLE). &lt;p&gt;Otherwise, MUSCLE re-arranges sequences so that similar sequences are adjacent in the output file. This makes the alignment easier to evaluate by eye. </source>
        <translation>Сохранять начальный порядок последовательностей в выравнивании. &lt;p&gt;Иначе, MUSCLE группирует похожие последовательности в выходном выравнивании, для удобства визуального сравнения.</translation>
    </message>
</context>
<context>
    <name>U2::LocalWorkflow::ProfileToProfileWorker</name>
    <message>
        <location filename="../src/ProfileToProfileWorker.cpp" line="112"/>
        <source>Aligned profile to profile with MUSCLE</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ProfileToProfileWorker.cpp" line="204"/>
        <source>Master profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ProfileToProfileWorker.cpp" line="205"/>
        <source>The main alignment which will be aligned on.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ProfileToProfileWorker.cpp" line="207"/>
        <source>Second profile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ProfileToProfileWorker.cpp" line="208"/>
        <source>Alignment which will be aligned to the master alignment.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ProfileToProfileWorker.cpp" line="221"/>
        <source>Align Profile to Profile With MUSCLE</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/ProfileToProfileWorker.cpp" line="222"/>
        <source>Aligns second profile to master profile with MUSCLE aligner.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>U2::MuscleAdapter</name>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="73"/>
        <source>No sequences in input file</source>
        <translation>Выравнивание не содержит данных</translation>
    </message>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="100"/>
        <source>alignment_is_empty</source>
        <translation>Выравнивание не содержит данных</translation>
    </message>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="51"/>
        <location filename="../src/MuscleAdapter.cpp" line="199"/>
        <location filename="../src/MuscleAdapter.cpp" line="284"/>
        <location filename="../src/MuscleAdapter.cpp" line="465"/>
        <source>Internal MUSCLE error: %1</source>
        <translation>Внутренняя ошибка MUSCLE: %1</translation>
    </message>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="277"/>
        <source>Invalid input alignment</source>
        <translation>Некорректное входное выравнивание</translation>
    </message>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="292"/>
        <location filename="../src/MuscleAdapter.cpp" line="473"/>
        <source>Incompatible alphabets</source>
        <translation>Несовместимые алфавиты</translation>
    </message>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="320"/>
        <source>Aligning profiles</source>
        <translation>Выравнивание</translation>
    </message>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="323"/>
        <source>Building output</source>
        <translation>Подготовка результата</translation>
    </message>
    <message>
        <source>Profile is not aligned</source>
        <translation type="obsolete">Выравнивание не нормализовано</translation>
    </message>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="498"/>
        <source>Aligning sequence %1 of %2</source>
        <translation>Выравнивается последовательность %1 из %2</translation>
    </message>
    <message>
        <location filename="../src/MuscleAdapter.cpp" line="530"/>
        <source>Merging results: %1 of %2</source>
        <translation>Объединяются результаты: %1 из %2</translation>
    </message>
    <message>
        <location filename="../src/MuscleUtils.cpp" line="91"/>
        <source>Unsupported alphabet: %1</source>
        <translation>Неподходящий алфавит: %1 </translation>
    </message>
</context>
<context>
    <name>U2::MuscleAddSequencesToProfileTask</name>
    <message>
        <location filename="../src/MuscleTask.cpp" line="263"/>
        <source>MUSCLE align profiles &apos;%1&apos; vs &apos;%2&apos;</source>
        <translation>MUSCLE выравнивает &apos;%1&apos; к &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="265"/>
        <source>MUSCLE align &apos;%2&apos; by profile &apos;%1&apos;</source>
        <translation>MUSCLE добавляет &apos;%2&apos; к &apos;%1&apos;</translation>
    </message>
    <message>
        <source>input_format_error</source>
        <translation type="obsolete">Не удалось определить формат файла</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="321"/>
        <source>Sequences in file have different alphabets %1</source>
        <translation>Последовательности в файле имеют разные алфавиты: %1</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="340"/>
        <source>No sequences found in file %1</source>
        <translation>Файл не содержит последовательностей: %1</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="342"/>
        <source>No alignment found in file %1</source>
        <translation>Файл не содержит выравниваний: %1</translation>
    </message>
</context>
<context>
    <name>U2::MuscleAlignDialogController</name>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="95"/>
        <source>Error</source>
        <translation>Ошибка</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="95"/>
        <source>Illegal alignment region</source>
        <translation>Неправильный регион</translation>
    </message>
    <message>
        <source>Selecting machines error!</source>
        <translation type="obsolete">Ошибка выбора удаленных машин!</translation>
    </message>
    <message>
        <source>You didn&apos;t select a machine to run remote task!</source>
        <translation type="obsolete">Не выбрана машина для удаленного запуска!</translation>
    </message>
    <message>
        <source>Distributed run on many machines is not supported yet. Select 1 machine</source>
        <translation type="obsolete">Распределенный запуск на нескольких машинах в данный пока не поддерживается. Выберите одну машину</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="241"/>
        <source>MUSCLE default</source>
        <translation>По умолчанию</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="242"/>
        <source>&lt;p&gt;The default settings are designed to give the best accuracy</source>
        <translation>&lt;p&gt;Наилучшая точность выравнивания</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="243"/>
        <source>&lt;p&gt;&lt;b&gt;Command line:&lt;/b&gt; muscle &lt;no-parameters&gt;</source>
        <translation>&lt;p&gt;&lt;b&gt;Командная строка:&lt;/b&gt; muscle &lt;no-parameters&gt;</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="247"/>
        <source>Large alignment</source>
        <translation>Большие выравнивания</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="248"/>
        <source>&lt;p&gt;If you have a large number of sequences (a few thousand), or they are very long, then the default settings may be too slow for practical use. A good compromise between speed and accuracy is to run just the first two iterations of the algorithm</source>
        <translation>&lt;p&gt;При наличии тысяч последовательностей либо их большой длине, конфигурация по умолчанию может оказаться неприемлемо медленной. Хороший компромисс между скоростью и точностью обеспечивается при прогоне только первых 2-х итераций алгоритма. </translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="249"/>
        <source>&lt;p&gt;&lt;b&gt;Command line:&lt;/b&gt; muscle &lt;i&gt;-maxiters 2&lt;/i&gt;</source>
        <translation>&lt;p&gt;&lt;b&gt;Командная строка:&lt;/b&gt; muscle &lt;i&gt;-maxiters 2&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="253"/>
        <source>Refine only</source>
        <translation>Только улучшить</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="254"/>
        <source>&lt;p&gt;Improves existing alignment without complete realignment</source>
        <translation>&lt;p&gt;Улучшение существующего выравнивания</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="255"/>
        <source>&lt;p&gt;&lt;b&gt;Command line:&lt;/b&gt; muscle &lt;i&gt;-refine&lt;/i&gt;</source>
        <translation>&lt;p&gt;&lt;b&gt;Командная строка:&lt;/b&gt; muscle &lt;i&gt;-refine&lt;/i&gt;</translation>
    </message>
</context>
<context>
    <name>U2::MuscleAlignWithExtFileSpecifyDialogController</name>
    <message>
        <source>Select input file</source>
        <translation type="obsolete">Выберите входной файл</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="151"/>
        <source>Open an alignment file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="161"/>
        <source>Save an multiple alignment file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="197"/>
        <source>Error</source>
        <translation type="unfinished">Ошибка</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="197"/>
        <source>Illegal alignment region</source>
        <translation type="unfinished">Неправильный регион</translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="214"/>
        <location filename="../src/MuscleAlignDialogController.cpp" line="217"/>
        <source>Kalign with Align</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="215"/>
        <source>Input file is not set!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleAlignDialogController.cpp" line="218"/>
        <source>Output file is not set!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>U2::MuscleGObjectRunFromSchemaTask</name>
    <message>
        <location filename="../src/MuscleTask.cpp" line="580"/>
        <source>MUSCLE align &apos;%1&apos;</source>
        <translation type="unfinished">MUSCLE выравнивает &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="583"/>
        <source>MUSCLE refine &apos;%1&apos;</source>
        <translation type="unfinished">MUSCLE улучшает &apos;%1&apos;</translation>
    </message>
</context>
<context>
    <name>U2::MuscleGObjectTask</name>
    <message>
        <location filename="../src/MuscleTask.cpp" line="372"/>
        <source>MUSCLE align &apos;%1&apos;</source>
        <translation>MUSCLE выравнивает &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="375"/>
        <source>MUSCLE refine &apos;%1&apos;</source>
        <translation>MUSCLE улучшает &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="378"/>
        <source>MUSCLE add to profile &apos;%1&apos;</source>
        <translation>MUSCLE добавляет в выравнивание &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="381"/>
        <source>MUSCLE align profiles</source>
        <translation>MUSCLE выравнивает пару выравниваний</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="396"/>
        <source>object_removed</source>
        <translation>Объект был удалён</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="400"/>
        <location filename="../src/MuscleTask.cpp" line="423"/>
        <source>object_is_state_locked</source>
        <translation>Объект заблокирован</translation>
    </message>
</context>
<context>
    <name>U2::MuscleLocalTask</name>
    <message>
        <source>Muscle local task</source>
        <translation type="obsolete">Внешняя задача выравнивания Muscle</translation>
    </message>
    <message>
        <source>No settings given</source>
        <translation type="obsolete">Не заданы настройки задачи</translation>
    </message>
    <message>
        <source>Muscle local task. Alignment: %1</source>
        <translation type="obsolete">Внешняя задача выравнивания Muscle. Множественное выравнивание:&apos;%1&apos;</translation>
    </message>
    <message>
        <source>Alien Muscle task from remote machine.</source>
        <translation type="obsolete">Внешняя задача выравнивания Muscle с удаленной машины.</translation>
    </message>
    <message>
        <source>Alignment</source>
        <translation type="obsolete">Множественное выравнивание</translation>
    </message>
    <message>
        <source>Task finished with error</source>
        <translation type="obsolete">Задача завершилась с ошибкой</translation>
    </message>
</context>
<context>
    <name>U2::MuscleMSAEditorContext</name>
    <message>
        <source>Align sequences to profile with MUSCLE</source>
        <translation type="obsolete">Выровнять с дополнительными последовательностями ...</translation>
    </message>
    <message>
        <source>Align profile to profile with MUSCLE</source>
        <translation type="obsolete">Выровнять с другим выравниванием ...</translation>
    </message>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="140"/>
        <source>Align with MUSCLE...</source>
        <translation>Выравнивание MUSCLE</translation>
    </message>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="148"/>
        <source>Align sequences to profile with MUSCLE...</source>
        <translation>Выровнять последовательности на профиль при помощи MUSCLE</translation>
    </message>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="155"/>
        <source>Align profile to profile with MUSCLE...</source>
        <translation>Выровнять профиль на профиль при помощи MUSCLE</translation>
    </message>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="226"/>
        <source>Select file with sequences</source>
        <translation>Выбор файла последовательностей</translation>
    </message>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="243"/>
        <source>Select file with alignment</source>
        <translation>Выбор файла выравнивания</translation>
    </message>
</context>
<context>
    <name>U2::MuscleParallelTask</name>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="49"/>
        <source>MuscleParallelTask</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>U2::MusclePlugin</name>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="60"/>
        <source>MUSCLE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="61"/>
        <source>A port of MUSCLE package for multiple sequence alignment. Check http://www.drive5.com/muscle/ for the original version</source>
        <translation>Порт пакета MUSCLE для выравнивания множественных последовательностей. ￼Сайт оригинального пакета http://www.drive5.com/muscle/</translation>
    </message>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="69"/>
        <source>MUSCLE...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MusclePlugin.cpp" line="75"/>
        <source>Multiple alignment</source>
        <translation type="unfinished">Множественное выравнивание</translation>
    </message>
    <message>
        <source>Multiple Alignment</source>
        <translation type="obsolete">Множественное Выравнивание</translation>
    </message>
</context>
<context>
    <name>U2::MusclePrepareTask</name>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="86"/>
        <source>Preparing MUSCLE alignment...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="94"/>
        <source>Internal parallel MUSCLE error: %1</source>
        <translation>Внутренняя ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="98"/>
        <source>MUSCLE prepared successfully</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="135"/>
        <source>No sequences in input file</source>
        <translation>Выравнивание не содержит данных</translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="162"/>
        <source>alignment_is_empty</source>
        <translation>Выравнивание не содержит данных</translation>
    </message>
</context>
<context>
    <name>U2::MuscleRemoteToGobjectTask</name>
    <message>
        <source>Malignment object removed</source>
        <translation type="obsolete">Объект множественного выравнивания удален</translation>
    </message>
    <message>
        <source>remote machine settings</source>
        <translation type="obsolete">настройки удаленной машины</translation>
    </message>
    <message>
        <source>MAlignment object is state locked</source>
        <translation type="obsolete">объект множественного выравнивания используется другой задачей</translation>
    </message>
    <message>
        <source>Cannot create remote machine</source>
        <translation type="obsolete">Не удалось создать удаленную машину</translation>
    </message>
    <message>
        <source>remote task didn&apos;t produced result</source>
        <translation type="obsolete">Удаленная задача не вернула результат</translation>
    </message>
    <message>
        <source>Muscle task runned on remote machine.</source>
        <translation type="obsolete">Задача Muscle, исполняющаяся на удаленной машине.</translation>
    </message>
    <message>
        <source>Alignment</source>
        <translation type="obsolete">Множественное выравнивание</translation>
    </message>
    <message>
        <source>Task finished with error</source>
        <translation type="obsolete">Задача завершилась с ошибкой</translation>
    </message>
</context>
<context>
    <name>U2::MuscleTask</name>
    <message>
        <location filename="../src/MuscleTask.cpp" line="75"/>
        <source>MUSCLE alignment</source>
        <translation>Выравнивание MUSCLE</translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="82"/>
        <source>MUSCLE alignment started</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="128"/>
        <source>Performing MUSCLE alignment...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/MuscleTask.cpp" line="150"/>
        <source>MUSCLE alignment successfully finished</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>U2::MuscleWithExtFileSpecifySupportTask</name>
    <message>
        <location filename="../src/MuscleTask.cpp" line="488"/>
        <source>input_format_error</source>
        <translation type="unfinished">Не удалось определить формат файла</translation>
    </message>
</context>
<context>
    <name>U2::ProgressiveAlignTask</name>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="274"/>
        <source>ProgressiveAlignTask</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="296"/>
        <source>Internal parallel MUSCLE error: %1</source>
        <translation>Внутренняя ошибка: %1</translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="301"/>
        <source>alignment &quot;%1&quot; Parallel MUSCLE Iter 1 accomplished. Time elapsed %2 ms</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>U2::ProgressiveAlignWorker</name>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="352"/>
        <source>ProgressiveAlignWorker</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="366"/>
        <source>Internal parallel MUSCLE error: %1</source>
        <translation>Внутренняя ошибка: %1</translation>
    </message>
</context>
<context>
    <name>U2::RefineTask</name>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="526"/>
        <source>RefineTask</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="549"/>
        <source>Internal parallel MUSCLE error: %1</source>
        <translation>Внутренняя ошибка: %1</translation>
    </message>
</context>
<context>
    <name>U2::RefineTreeTask</name>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="481"/>
        <source>RefineTreeTask</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="493"/>
        <source>Internal parallel MUSCLE error: %1</source>
        <translation>Внутренняя ошибка: %1</translation>
    </message>
</context>
<context>
    <name>U2::RefineWorker</name>
    <message>
        <location filename="../src/MuscleParallel.cpp" line="602"/>
        <source>Internal parallel MUSCLE error: %1</source>
        <translation type="unfinished">Внутренняя ошибка: %1</translation>
    </message>
</context>
</TS>
