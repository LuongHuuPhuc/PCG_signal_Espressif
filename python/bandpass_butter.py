import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

# ======================
# THAM SỐ
# ======================
fs = 4000.0        # Tần số lấy mẫu (Hz)
order = 4          # Bậc lọc

lowcut = 100.0
highcut = 200.0

# ======================
# HÀM BANDPASS FILTER
# ======================
def bandpass_filter(x, fs, lowcut, highcut, order=4):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype="band")
    return filtfilt(b, a, x)

# ======================
# ĐỌC FILE CSV (1 CỘT, KHÔNG HEADER)
# ======================
file_path = r"D:\Esp-idf\Test_source\inmp441_test\data_text\Snore_test\snore5_4kHzsr.csv"

# đọc 1 cột
df = pd.read_csv(file_path, header=None)     # shape (N,1)
pcg = pd.to_numeric(df.iloc[:, 0], errors="coerce").dropna().to_numpy()

# ======================
# LỌC
# ======================
pcg_filt = bandpass_filter(pcg, fs, lowcut, highcut, order)

# ======================
# VẼ
# ======================
plt.figure(figsize=(12, 4))
plt.plot(pcg, alpha=0.5, label="Raw")
plt.plot(pcg_filt, label="Filtered")
plt.title(f"PCG Bandpass {lowcut}-{highcut} Hz (fs={fs} Hz)")
plt.xlabel("Sample index")
plt.ylabel("Amplitude")
plt.legend()
plt.tight_layout()
plt.show()
