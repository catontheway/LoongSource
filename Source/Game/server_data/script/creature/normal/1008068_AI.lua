--��������ű�����ID��1008068
--����ʱ ԭ��ˢ�³���������ױ�1008012

--����ʱ�ű�
function c1008068_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)

	map.MapCreateCreature(MapID, InstanceID, 1008012, x, y+1, z)

end
--ע��
aux.RegisterCreatureEvent(1008068, 4, "c1008068_OnDie")


-- �������ս������ʾ��
function c1008068_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008068, 2, "c1008068_OnEnterCombat")
