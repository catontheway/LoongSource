--��ǿ���ǣ�10847
--����1���������ʱ������ҵȼ���ӻظ�BUFF

--�ظ�buff��
x010847_HellBuffID  =   {[1]     =   2012701,    [2]     =   2012701,    [3]     =   2012702,
                         [4]     =   2012703,    [5]     =   2012704,    [6]     =   2012705,
                         [7]     =   2012706,    [8]     =   2012707,    [9]     =   2012708,
                         [10]    =   2012709,    [11]	=   2012710,    [12]	=   2012710,
                         [13]    =   2012710,    [14]	=   2012710,    [15]	=   2012710,
                         [16]    =   2012710
                       }

--�����ύʱ
function x10847_OnComplete(mapID, instanceID, questID, ownerID, accepterID)
    --��ҵȼ�/10+1Ϊ����buff������
	local k = math.floor(role.GetRoleLevel(mapID, instanceID, ownerID)/10) + 1
	unit.AddBuff(mapID, instanceID, ownerID, x010847_HellBuffID[k], ownerID)
end

--ע��
aux.RegisterQuestEvent(10847, 1, "x10847_OnComplete")
