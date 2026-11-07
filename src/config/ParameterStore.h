#pragma once

#include "IniConfigStore.h"
#include "ParameterCodec.h"

namespace fosd {
namespace config {

// Typed application parameters; generic INI storage remains independent.
class ParameterStore {
public:
    // Uses the runtime INI path unless an explicit file path is supplied.
    explicit ParameterStore(const QString &path = runtimePath());
    // FOSD_CONFIG_FILE overrides <application directory>/config/parameters.ini.
    static QString runtimePath();
    // Fill missing defaults, validate the merged section, then load persisted values.
    // Existing values are preserved; failure leaves the output unchanged.
    template <class T> bool initialize(T *value, QString *error = nullptr) {
        if (!value) {
            if (error)
                *error = "Null parameter output";
            return false;
        }
        if (!prepare(error))
            return false;
        ConfigSection current;
        ConfigData all;
        if (!m_store.readAll(&all, error))
            return false;
        current = all.value(ParameterCodec<T>::section());
        const auto defaults = parameterSection(*value);
        ConfigSection merged = defaults;
        for (auto it = current.constBegin(); it != current.constEnd(); ++it)
            merged[it.key()] = it.value();
        T parsed;
        if (!decodeParameter(merged, &parsed, false, error))
            return false;
        if (!m_store.ensureModule(ParameterCodec<T>::section(), defaults, error))
            return false;
        return read(value, false, error);
    }
    // Read a complete section; ready also checks operation prerequisites.
    template <class T> bool read(T *value, bool ready = false, QString *error = nullptr) const {
        if (!value) {
            if (error)
                *error = "Null parameter output";
            return false;
        }
        ConfigSection section;
        return m_store.readModule(ParameterCodec<T>::section(), &section, error) &&
               decodeParameter(section, value, ready, error);
    }
    // Validate before replacing one section atomically; retain all other sections.
    template <class T> bool write(const T &value, bool ready = true, QString *error = nullptr) {
        T checked;
        const auto section = parameterSection(value);
        if (!decodeParameter(section, &checked, ready, error) || !prepare(error))
            return false;
        return m_store.writeModule(ParameterCodec<T>::section(), section, error);
    }

private:
    // Seed a missing runtime file and reject writes to the default template.
    bool prepare(QString *error);
    IniConfigStore m_store;
};
} // namespace config
} // namespace fosd
