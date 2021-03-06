#ifndef _U2_TEST_RUNNER_SETTINGS_H_
#define _U2_TEST_RUNNER_SETTINGS_H_


namespace U2 {

class TestRunnerSettings {
public:
    QString getVar(const QString& name) const { return registry.value(name); }
    void setVar(const QString& name, const QString& val) { registry[name] = val; }
    void removeVar(const QString& name) { registry.remove(name); }
private:
    QMap<QString, QString> registry;
};

class APITestData {
public:
    template<class T>
    T getValue(const QString& key) const {
        const QVariant& val = d.value(key);
        return val.value<T>();
    }

    template<class T>
    void addValue(const QString& key, const T& val) {
        assert(!key.isEmpty());
        assert(!d.keys().contains(key));
        const QVariant& var = qVariantFromValue<T>(val);
        d[key] = var;
    }

    template<class T>
    QList<T> getList(const QString& key) const {
        const QVariant& val = d.value(key);
        if (val.type() != QVariant::List) {
            return QList<T>();
        }
        const QVariantList& varList = val.toList();
        QList<T> list;
        foreach(const QVariant& var, varList) {
            list << var.value<T>();
        }
        return list;
    }

    template<class T>
    void addList(const QString& key, const QList<T>& list) {
        assert(!key.isEmpty());
        assert(!d.keys().contains(key));
        QVariantList varList;
        foreach(const T& val, list) {
            varList << qVariantFromValue<T>(val);
        }
        d[key] = varList;
    }

private:
    QMap<QString, QVariant> d;
};

} //namespace

#endif
