using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore.Utils
{
    public static class DateUtil
    {
        public static DateTime GetDateTime()
        {
            return DateTime.UtcNow.AddHours(9);
        }

        public static int GetTotalMinutes()
        {
            return (int)GetDateTime().TimeOfDay.TotalMinutes;

        }

        public static bool IsRefresh(DateTime refresh_date)
        {
            if (GetTotalMinutes() >= 540 && refresh_date <= GetDateTime().Date)
            {
                return true;
            }

            return false;
        }

        public static int WeekDay(DateTime dateTime)
        {
            int tempWeekDay = 0;

            var dt = dateTime.DayOfWeek;

            switch (dt)
            {
                case DayOfWeek.Monday: //월
                    tempWeekDay = 1;
                    break;

                case DayOfWeek.Tuesday: //화
                    tempWeekDay = 2;
                    break;

                case DayOfWeek.Wednesday: //수
                    tempWeekDay = 3;
                    break;

                case DayOfWeek.Thursday: //목
                    tempWeekDay = 4;
                    break;

                case DayOfWeek.Friday: //금
                    tempWeekDay = 5;
                    break;

                case DayOfWeek.Saturday: //토
                    tempWeekDay = 6;
                    break;

                case DayOfWeek.Sunday: //일
                    tempWeekDay = 7;
                    break;

            }

            return tempWeekDay;
        }
    }
}
