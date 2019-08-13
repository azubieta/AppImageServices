#pragma once

/**
 * Tries to forward the execution of the AppImage pointed by <appImagePath> the the integration assistant. This could
 * be used to show first time run dialogs.
 *
 * @param appImagePath
 * @param args
 * @return true if the integration assistant was properly executed, false otherwise.
 */
int tryForwardExecToIntegrationAssistant(int argc, char* const* argv, char* appImagePath);


