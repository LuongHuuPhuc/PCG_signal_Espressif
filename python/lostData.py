import pandas as pd
import matplotlib.pyplot as plt

data_fame = pd.read_csv("D:\Esp-idf\Mysource\sph0645_test\data_test400Hz_1st.csv")
x = range(len(data_fame))
plt.plot(x, data_fame, marker="x", linestyle='-')

plt.xlabel('Chỉ số dòng')
plt.ylabel('Biên độ')
plt.title('Biểu đồ dữ liệu từ file CSV')

plt.show()  